namespace detail
{

template<typename InputIt>
size_t getUnicodeBytesCount(InputIt begin, InputIt end)
{
	size_t n = 0;

	while (begin != end) {
		if (*begin >= 0)
			++begin;
		else {
			if ((*begin & 0xF0) == 0xF0)
				begin += 4;
			else if ((*begin & 0xE0) == 0xE0)
				begin += 3;
			else
				begin += 2;
		}
		++n;
	}

	return n;
}

template<typename InputIt, typename OutputIt>
void utf8ToUnicode(InputIt begin, InputIt end, OutputIt dst)
{
	uint32_t helper;
	uint32_t aux;

	while (begin != end) {
		if (*begin >= 0) {
			*dst = *begin;
			++begin;
		}
		else {
			helper = 0;
			if ((*begin & 0xF0) == 0xF0) {
				aux = *begin; aux &= 0x07; aux <<= 18; helper |= aux; ++begin;
				aux = *begin; aux &= 0x3F; aux <<= 12; helper |= aux; ++begin;
				aux = *begin; aux &= 0x3F; aux <<=  6; helper |= aux; ++begin;
				aux = *begin; aux &= 0x3F;             helper |= aux; ++begin;
				*dst = helper;
			}
			else if ((*begin & 0xE0) == 0xE0) {
				aux = *begin; aux &= 0x0F; aux <<= 12; helper |= aux; ++begin;
				aux = *begin; aux &= 0x3F; aux <<=  6; helper |= aux; ++begin;
				aux = *begin; aux &= 0x3F;             helper |= aux; ++begin;
				*dst = helper;
			}
			else {
				aux = *begin; aux &= 0x1F; aux <<=  6; helper |= aux; ++begin;
				aux = *begin; aux &= 0x3F;             helper |= aux; ++begin;
				*dst = helper;
			}
		}
		++dst;
	}
}

}
