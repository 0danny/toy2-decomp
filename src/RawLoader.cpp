#include "RawLoader.h"

#define LOBYTE_RD(x) (*((uint8_t*)&(x)))
#define HIBYTE_RD(x) (*((uint8_t*)&(x) + 1))

#define LOWORD_RD(x) (*((uint16_t*)&(x)))
#define HIWORD_RD(x) (*((uint16_t*)&(x) + 1))

namespace RawLoader
{

	// FUNCTION: TOY2 0x0047B170
	void DecompressBuffer(uint8_t* p_inBuffer, uint8_t* p_outBuffer)
	{
		// Decompression is based on RNC ProPack Method 2. Normally these files
		// have a standard 18 byte header, which looks like it has been replaced
		// in Toy 2 with a custom 15 byte header.

		uint32_t copyLength;
		uint32_t exitFlag;
		uint32_t lengthCode;
		uint32_t distanceHighOrLiteralCount;
		uint32_t extendedLengthBit;
		uint32_t lengthFollowupBit;
		uint32_t hasDistanceHighBits;

		uint8_t* afterHeader = p_inBuffer + 15;
		uint32_t bitAccum = 4 * p_inBuffer[14] + 2;

		do
		{
			while (true)
			{
				while (true)
				{
					while (true)
					{
						bitAccum *= 2;
						uint32_t bitTest = (bitAccum >> 8) & 1;

						if (! bitTest)
						{
							bitAccum *= 2;

							*p_outBuffer++ = *afterHeader++;

							bitTest = (bitAccum >> 8) & 1;
						}

						if (bitTest)
						{
							bitAccum &= 0xFF;

							if (bitAccum)
								break;

							bitAccum = bitTest + 2 * (*afterHeader++);

							if ((bitAccum & 0x100) != 0)
								break;
						}

						*p_outBuffer++ = *afterHeader++;
					}

					LOBYTE_RD(lengthCode) = 2 * bitAccum;
					distanceHighOrLiteralCount = 0;
					copyLength = 2;

					uint32_t lengthCodeBit = ((2 * bitAccum) >> 8) & 1;
					lengthCode &= 0xFF;

					if (! lengthCode)
					{
						lengthCode = lengthCodeBit + 2 * (*afterHeader++);
						lengthCodeBit = (lengthCode >> 8) & 1;
					}

					if (lengthCodeBit)
						break;

					uint32_t shiftedLengthCode = 2 * lengthCode;
					uint32_t shiftedLengthBit = (shiftedLengthCode >> 8) & 1;
					shiftedLengthCode &= 0xFF;

					if (! shiftedLengthCode)
					{
						LOBYTE_RD(lengthCodeBit) = (*afterHeader++);
						shiftedLengthCode = shiftedLengthBit + 2 * lengthCodeBit;
						shiftedLengthBit = (shiftedLengthCode >> 8) & 1;
					}

					LOBYTE_RD(bitAccum) = 2 * shiftedLengthCode;
					copyLength = shiftedLengthBit + 4;

					uint32_t extraLengthBit = ((2 * shiftedLengthCode) >> 8) & 1;

					bitAccum &= 0xFF;

					if (! bitAccum)
					{
						bitAccum = extraLengthBit + 2 * (*afterHeader++);
						extraLengthBit = (bitAccum >> 8) & 1;
					}

					if (! extraLengthBit)
						goto LBL_DECODE_DISTANCE;

					bitAccum *= 2;
					uint32_t finalLengthBit = (bitAccum >> 8) & 1;
					bitAccum &= 0xFF;

					if (! bitAccum)
					{
						bitAccum = finalLengthBit + 2 * (*afterHeader++);
						finalLengthBit = (bitAccum >> 8) & 1;
					}

					copyLength = finalLengthBit + 2 * (copyLength - 1);

					if (copyLength != 9)
						goto LBL_DECODE_DISTANCE;

					for (int32_t bitIndex = 3; bitIndex >= 0; --bitIndex)
					{
						bitAccum *= 2;
						uint32_t literalCountBit = (bitAccum >> 8) & 1;
						bitAccum &= 0xFF;

						if (! bitAccum)
						{
							bitAccum = literalCountBit + 2 * (*afterHeader++);
							literalCountBit = (bitAccum >> 8) & 1;
						}

						distanceHighOrLiteralCount = literalCountBit + 2 * distanceHighOrLiteralCount;
					}

					*p_outBuffer = *afterHeader;
					p_outBuffer[1] = afterHeader[1];

					p_outBuffer += 4;
					afterHeader += 4;

					*(p_outBuffer - 2) = afterHeader[2];
					*(p_outBuffer - 1) = afterHeader[3];

					uint32_t literalBlockCount = distanceHighOrLiteralCount + 1;

					if (literalBlockCount >= 0)
					{
						uint32_t literalBlocksRemaining = literalBlockCount + 1;

						do
						{
							*p_outBuffer = *afterHeader;
							p_outBuffer[1] = afterHeader[1];
							p_outBuffer += 4;

							*(p_outBuffer - 2) = afterHeader[2];
							*(p_outBuffer - 1) = afterHeader[3];

							afterHeader += 4;
							--literalBlocksRemaining;
						} while (literalBlocksRemaining);
					}
				}

				LOBYTE_RD(bitAccum) = 2 * lengthCode;
				lengthFollowupBit = ((2 * lengthCode) >> 8) & 1;
				bitAccum &= 0xFF;

				if (! bitAccum)
				{
					bitAccum = lengthFollowupBit + 2 * (*afterHeader++);
					lengthFollowupBit = (bitAccum >> 8) & 1;
				}

				if (! lengthFollowupBit)
					goto LBL_COPY_MATCH;

				bitAccum *= 2;
				copyLength = 3;

				extendedLengthBit = (bitAccum >> 8) & 1;
				bitAccum &= 0xFF;

				if (! bitAccum)
				{
					bitAccum = extendedLengthBit + 2 * (*afterHeader++);
					extendedLengthBit = (bitAccum >> 8) & 1;
				}

				if (extendedLengthBit)
					break;

			LBL_DECODE_DISTANCE:

				bitAccum *= 2;
				hasDistanceHighBits = (bitAccum >> 8) & 1;
				bitAccum &= 0xFF;

				if (! bitAccum)
				{
					bitAccum = hasDistanceHighBits + 2 * (*afterHeader++);
					hasDistanceHighBits = (bitAccum >> 8) & 1;
				}

				if (hasDistanceHighBits)
				{
					uint32_t distancePrefixAccum = 2 * bitAccum;
					uint32_t distancePrefixBit = (distancePrefixAccum >> 8) & 1;

					distancePrefixAccum &= 0xFF;

					if (! distancePrefixAccum)
					{
						distancePrefixAccum = distancePrefixBit + 2 * (*afterHeader++);
						distancePrefixBit = (distancePrefixAccum >> 8) & 1;
					}

					bitAccum = 2 * distancePrefixAccum;
					uint32_t distanceHighBits = distancePrefixBit;

					uint32_t hasMoreDistanceBits = (bitAccum >> 8) & 1;
					bitAccum &= 0xFF;

					if (! bitAccum)
					{
						bitAccum = hasMoreDistanceBits + 2 * (*afterHeader++);
						hasMoreDistanceBits = (bitAccum >> 8) & 1;
					}

					if (hasMoreDistanceBits)
					{
						uint32_t shifted = 2 * bitAccum;
						uint32_t distanceExtraBit = (shifted >> 8) & 1;
						uint32_t distanceExtraAccum = shifted & 0xFF;

						if (! distanceExtraAccum)
						{
							distanceExtraAccum = distanceExtraBit + 2 * (*afterHeader++);
							distanceExtraBit = (distanceExtraAccum >> 8) & 1;
						}

						distanceHighBits = (distanceExtraBit + 2 * distanceHighBits) | 4;

						shifted = 2 * distanceExtraAccum;
						uint32_t distanceTerminatorBit = (shifted >> 8) & 1;
						bitAccum = shifted & 0xFF;

						if (! bitAccum)
						{
							bitAccum = distanceTerminatorBit + 2 * (*afterHeader++);
							distanceTerminatorBit = (bitAccum >> 8) & 1;
						}

						if (distanceTerminatorBit)
							goto LBL_COMMIT_DISTANCE;
					}
					else
					{
						if (distanceHighBits)
						{
						LBL_COMMIT_DISTANCE:

							int16_t distanceHighWord = 0;
							HIBYTE_RD(distanceHighWord) = distanceHighBits;
							LOWORD_RD(distanceHighOrLiteralCount) = distanceHighWord | (distanceHighBits >> 8);

							goto LBL_COPY_MATCH;
						}

						distanceHighBits = 1;
					}

					bitAccum *= 2;
					uint32_t distanceTailBit = (bitAccum >> 8) & 1;
					bitAccum &= 0xFF;

					if (! bitAccum)
					{
						bitAccum = distanceTailBit + 2 * (*afterHeader++);
						distanceTailBit = (bitAccum >> 8) & 1;
					}

					distanceHighBits = distanceTailBit + 2 * distanceHighBits;
					goto LBL_COMMIT_DISTANCE;
				}

			LBL_COPY_MATCH:

				int32_t matchDistance = distanceHighOrLiteralCount & 0xFF00 | (*afterHeader++);
				uint8_t* matchSource = &p_outBuffer[-matchDistance - 1];

				if (copyLength & 1)
				{
					matchSource = &p_outBuffer[-matchDistance];
					*p_outBuffer++ = *matchSource;
				}

				int32_t pairCopyCount = (copyLength >> 1) - 1;

				if (matchDistance)
				{
					p_outBuffer += 2;
					uint16_t* matchSourceWords = (uint16_t*)(matchSource + 2);

					*(p_outBuffer - 1) = *((uint8_t*)matchSourceWords - 1);
					*(p_outBuffer - 2) = *matchSource;

					int32_t wordCopyLoopCount = pairCopyCount - 1;

					if (wordCopyLoopCount >= 0)
					{
						uint32_t wordCopiesRemaining = wordCopyLoopCount + 1;

						do
						{
							*(uint16_t*)p_outBuffer = *matchSourceWords++;
							p_outBuffer += 2;
							--wordCopiesRemaining;
						} while (wordCopiesRemaining);
					}
				}
				else
				{
					p_outBuffer += 2;
					uint8_t repeatedByte = *matchSource;
					int32_t repeatPairLoopCount = pairCopyCount - 1;

					*(p_outBuffer - 2) = repeatedByte;
					*(p_outBuffer - 1) = repeatedByte;

					if (repeatPairLoopCount >= 0)
					{
						int32_t repeatPairsRemaining = repeatPairLoopCount + 1;

						do
						{
							*p_outBuffer = repeatedByte;
							p_outBuffer[1] = repeatedByte;
							p_outBuffer += 2;
							--repeatPairsRemaining;
						} while (repeatPairsRemaining);
					}
				}
			}

			int32_t extendedLengthByte = (*afterHeader++);

			if (extendedLengthByte)
			{
				copyLength = extendedLengthByte + 8;
				goto LBL_DECODE_DISTANCE;
			}

			bitAccum *= 2;
			exitFlag = (bitAccum >> 8) & 1;
			bitAccum &= 0xFF;

			if (! bitAccum)
			{
				bitAccum = exitFlag + 2 * (*afterHeader++);
				exitFlag = (bitAccum >> 8) & 1;
			}
		} while (exitFlag);
	}

	// STUB: TOY2 0x00452310
	void LoadPacketData(char* fileName) {}

}