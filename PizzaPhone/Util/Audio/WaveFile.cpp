#include "WaveFile.h"

namespace Util {
    namespace Audio {
        WaveFile::WaveFile(std::wstring const & filename) {
        }

        WaveFile::~WaveFile(void) {
        }

        void WaveFile::ReadHeader() {
            //char ch[5];
            //spx_int32_t itmp;
            //spx_int16_t stmp;
            //spx_int32_t bpersec;
            //spx_int16_t balign;
            //int skip_bytes;
            //int i;

            //ch[4]=0;
#if 0
            fread(ch, 1, 4, file);
            if (strcmp(ch, "RIFF")!=0) {
                fseek(file, 0, SEEK_SET);
                return 0;
            }

            fread(&itmp, 4, 1, file);
            *size = le_int(itmp-36);

            fread(ch, 1, 4, file);
            if (strcmp(ch, "WAVE")!=0) {
                fprintf (stderr, "RIFF file is not a WAVE file\n");
                return -1;
            }
#endif
            //fread(ch, 1, 4, file);
            //while (strcmp(ch, "fmt ")!=0) {
            //  fread(&itmp, 4, 1, file);
            //  itmp = le_int(itmp);
            //  /*fprintf (stderr, "skip=%d\n", itmp);*/
            //  /*strange way of seeking, but it works even for pipes*/
            //  for (i=0;i<itmp;i++) {
            //      fgetc(file);
            //  }
            //  /*fseek(file, itmp, SEEK_CUR);*/
            //  fread(ch, 1, 4, file);
            //  if (feof(file)) {
            //      fprintf (stderr, "Corrupted WAVE file: no \"fmt \"\n");
            //      return -1;
            //  }
            //}
            ///*
            //if (strcmp(ch, "fmt ")!=0) {
            //  fprintf (stderr, "Corrupted WAVE file: no \"fmt \"\n");
            //  return -1;
            //}
            //*/

            //fread(&itmp, 4, 1, file);
            //itmp = le_int(itmp);
            //skip_bytes=itmp-16;
            ///*fprintf (stderr, "skip=%d\n", skip_bytes);*/

            //fread(&stmp, 2, 1, file);
            //stmp = le_short(stmp);
            //if (stmp!=1) {
            //  fprintf (stderr, "Only PCM encoding is supported\n");
            //  return -1;
            //}

            //fread(&stmp, 2, 1, file);
            //stmp = le_short(stmp);
            //*channels = stmp;

            //if (stmp>2) {
            //  fprintf (stderr, "Only mono and (intensity) stereo supported\n");
            //  return -1;
            //}

            //fread(&itmp, 4, 1, file);
            //itmp = le_int(itmp);
            //*rate = itmp;
            //if (*rate != 8000 && *rate != 16000 && *rate != 11025 && *rate != 22050 && *rate != 32000 && *rate != 44100 && *rate != 48000) {
            //  fprintf (stderr, "Only 8 kHz (narrowband) and 16 kHz (wideband) supported (plus 11.025 kHz and 22.05 kHz, but your mileage may vary)\n");
            //  return -1;
            //}

            //fread(&itmp, 4, 1, file);
            //bpersec = le_int(itmp);

            //fread(&stmp, 2, 1, file);
            //balign = le_short(stmp);

            //fread(&stmp, 2, 1, file);
            //stmp = le_short(stmp);
            //if (stmp!=16 && stmp!=8) {
            //  fprintf (stderr, "Only 8/16-bit linear supported\n");
            //  return -1;
            //}
            //*format=stmp;

            //if (bpersec!=*rate**channels*stmp/8) {
            //  fprintf (stderr, "Corrupted header: ByteRate mismatch\n");
            //  return -1;
            //}

            //if (balign!=*channels*stmp/8) {
            //  fprintf (stderr, "Corrupted header: BlockAlign mismatch\n");
            //  return -1;
            //}


            ///*strange way of seeking, but it works even for pipes*/
            //if (skip_bytes>0) {
            //  for (i=0;i<skip_bytes;i++) {
            //      fgetc(file);
            //  }
            //}

            ///*fseek(file, skip_bytes, SEEK_CUR);*/

            //fread(ch, 1, 4, file);
            //while (strcmp(ch, "data")!=0) {
            //  fread(&itmp, 4, 1, file);
            //  itmp = le_int(itmp);
            //  /*strange way of seeking, but it works even for pipes*/
            //  for (i=0;i<itmp;i++) {
            //      fgetc(file);
            //  }
            //  /*fseek(file, itmp, SEEK_CUR);*/
            //  fread(ch, 1, 4, file);
            //  if (feof(file)) {
            //      fprintf (stderr, "Corrupted WAVE file: no \"data\"\n");
            //      return -1;
            //  }
            //}

            ///*Ignore this for now*/
            //fread(&itmp, 4, 1, file);
            //itmp = le_int(itmp);

            //*size=itmp;

            //return 1;
        }

        void WaveFile::WriteHeader() {
            //char ch[5];
            //spx_int32_t itmp;
            //spx_int16_t stmp;

            //ch[4]=0;

            //fprintf (file, "RIFF");

            //itmp = 0x7fffffff;
            //fwrite(&itmp, 4, 1, file);

            //fprintf (file, "WAVEfmt ");

            //itmp = le_int(16);
            //fwrite(&itmp, 4, 1, file);

            //stmp = le_short(1);
            //fwrite(&stmp, 2, 1, file);

            //stmp = le_short(channels);
            //fwrite(&stmp, 2, 1, file);

            //itmp = le_int(rate);
            //fwrite(&itmp, 4, 1, file);

            //itmp = le_int(rate*channels*2);
            //fwrite(&itmp, 4, 1, file);

            //stmp = le_short(2*channels);
            //fwrite(&stmp, 2, 1, file);

            //stmp = le_short(16);
            //fwrite(&stmp, 2, 1, file);

            //fprintf (file, "data");

            //itmp = le_int(0x7fffffff);
            //fwrite(&itmp, 4, 1, file);
        }
    }
}
