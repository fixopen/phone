#include "ByteOperator.h"
#include <algorithm> //for std::swap
//#include <utility> //for std::swap

namespace Util {
	namespace ByteOperator {
		unsigned int const ReverseByte(unsigned int const v) {
			unsigned int result = 0;
			union Bytes {
				unsigned int whole;
				unsigned char parts[4];
			};
			Bytes place;
			place.whole = v;
			std::swap(place.parts[0], place.parts[3]);
			std::swap(place.parts[1], place.parts[2]);
			result = place.whole;
			return result;
		}
	}
}
