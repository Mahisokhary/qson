#include "deserialize.internal.h"

inline static qson_result_t handle_escape(struct qson_deserialize_ctx *c, char *buffer, int *sizep, int *ip) {
	int size = *sizep;
	int i = *ip;
	char escaped = c->buffer[++c->index];

	switch (escaped) {
	case 'u':
		unsigned int codepoint;
		if (sscanf(c->buffer + ++c->index, "%4x", &codepoint) != 1) return QSON_RESULT_INVALID_CHAR;
		if (codepoint <= 0x7F) {
			buffer[i] = (unsigned char) codepoint;
		} else if (codepoint <= 0x7FF) {
			if (size - i < 2) return QSON_RESULT_BUFFER_TOO_SMALL;
			buffer[++i] = 0xC0 | (codepoint >> 6);
			buffer[++i] = 0x80 | (codepoint & 0x3F);
			i++;
		} else if (codepoint <= 0xFFFF) {
			if (size - i < 3) return QSON_RESULT_BUFFER_TOO_SMALL;
			buffer[i] = 0xE0 | (codepoint >> 12);
			buffer[++i] = 0x80 | ((codepoint >> 6) & 0x3F);
			buffer[++i] = 0x80 | (codepoint & 0x3F);
		}
		c->index += 3;
		break;
	case 'b': buffer[i] = '\b'; break;
	case 'f': buffer[i] = '\f'; break;
	case 'n': buffer[i] = '\n'; break;
	case 'r': buffer[i] = '\r'; break;
	case 't': buffer[i] = '\t'; break;
	default : buffer[i] = escaped; break;
	}

	*ip = i;
	return QSON_RESULT_OK;
}

qson_result_t qson_read_string(qson_deserialize_ctx_t ctx, char *buffer, int *sizep) {
	struct qson_deserialize_ctx *c = ctx;
	if (c->buffer[c->index] != QSON_QUOTATION_MARK) return QSON_RESULT_INVALID_CHAR;
	qson_ctx_skip(c, 1);

	int size = *sizep;
	int i = 0;
	while (i < size) {
		char chr = c->buffer[c->index];
		switch (chr) {
		case QSON_QUOTATION_MARK:
			buffer[i] = '\0';
			*sizep = ++i;
			c->index++;
			return QSON_RESULT_OK;
		case QSON_STRING_ESCAPE_CHAR:
			qson_result_t res = handle_escape(c, buffer, sizep, &i);
			if (res != QSON_RESULT_OK) return res;
			break;
		default:
			if (chr < 32 || 126 < chr) return QSON_RESULT_INVALID_CHAR;
			buffer[i] = chr;
		}
		qson_ctx_skip(c, 1);
		i++;
	}
	return QSON_RESULT_BUFFER_TOO_SMALL;
}

