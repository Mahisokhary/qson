#include "deserialize.internal.h"

#define BUFFER_SIZE_DEFAULT 8

struct buffer {
	qson_deserialize_ctx_t c;
	bool dynamic;	// Is buffer resizable
	size_t index;	// Current index
	size_t size;	// Its capacity
	char *buffer;	// Buffer
};

inline static qson_result_t buffer_write(struct buffer *b, char c) {
	if (b->size - b->index) {
		b->buffer[b->index++] = c;
	} else if (b->dynamic) {
		b->size *= 2;
		b->buffer = qrealloc(b->c, b->buffer, b->size);
		return buffer_write(b, c);
	} else {
		return QSON_RESULT_BUFFER_TOO_SMALL;
	}
	return  QSON_RESULT_OK;
}

inline static qson_result_t handle_escape(struct buffer *b) {
	qson_deserialize_ctx_t c = b->c;
	char escaped = c->buffer[++c->index];

	switch (escaped) {
	case 'u':
		unsigned int codepoint;
		qson_ctx_size_check(c, 4);
		c->index += 1;
		if (sscanf(c->buffer + c->index, "%4x", &codepoint) != 1) return QSON_RESULT_INVALID_CHAR;
		if (codepoint <= 0x7F) {
			buffer_write(b, codepoint);
		} else if (codepoint <= 0x7FF) {
			buffer_write(b, 0xC0 | (codepoint >> 6));
			buffer_write(b, 0x80 | (codepoint & 0x3F));
		} else if (codepoint <= 0xFFFF) {
			buffer_write(b, 0xE0 | (codepoint >> 12));
			buffer_write(b, 0x80 | ((codepoint >> 6) & 0x3F));
			buffer_write(b, 0x80 | (codepoint & 0x3F));
		}
		c->index += 3;
		break;
	case 'b': buffer_write(b, '\b'); break;
	case 'f': buffer_write(b, '\f'); break;
	case 'n': buffer_write(b, '\n'); break;
	case 'r': buffer_write(b, '\r'); break;
	case 't': buffer_write(b, '\t'); break;
	default : buffer_write(b, escaped); break;
	}
	return QSON_RESULT_OK;
}

inline static qson_result_t _qson_deserialize_string(struct buffer *b) {
	qson_deserialize_ctx_t c = b->c;
	if (c->buffer[c->index] != QSON_QUOTATION_MARK) return QSON_RESULT_INVALID_CHAR;
	qson_ctx_skip(c, 1);

	while (true) {
		char chr = c->buffer[c->index];
		switch (chr) {
		case QSON_QUOTATION_MARK:
			buffer_write(b, '\0');
			qson_ctx_skip(c, 1);
			return QSON_RESULT_OK;
		case QSON_STRING_ESCAPE_CHAR:
			qson_result_t res = handle_escape(b);
			if (res != QSON_RESULT_OK) return res;
			break;
		default:
			if (chr < 32 || 126 < chr) return QSON_RESULT_INVALID_CHAR;
			buffer_write(b, chr);
		}
		qson_ctx_skip(c, 1);
	}
	return QSON_RESULT_OK;
}

qson_result_t qson_deserialize_string_auto(qson_deserialize_ctx_t c, char **buffer, size_t *sizep) {
	struct buffer b = { c, true, 0, BUFFER_SIZE_DEFAULT, NULL };
	b.buffer = qmalloc(c, b.size);
	qson_run(_qson_deserialize_string(&b));
	*buffer = qrealloc(c, b.buffer, b.index);
	*sizep = b.index;
	return QSON_RESULT_OK;
}

qson_result_t qson_deserialize_string(qson_deserialize_ctx_t c, char *buffer, int *sizep) {
	struct buffer b = { c, false, 0, *sizep, buffer };
	qson_run(_qson_deserialize_string(&b));
	*sizep = b.index;
	return QSON_RESULT_OK;
}

