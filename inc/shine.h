#ifndef SHINE_H
#define SHINE_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "types.h"

	/* Side information */
	typedef struct {
        unsigned part2_3_length;
        unsigned big_values;
        unsigned count1;
        unsigned global_gain;
        unsigned scalefac_compress;
        unsigned table_select[3];
        unsigned region0_count;
        unsigned region1_count;
        unsigned preflag;
        unsigned scalefac_scale;
        unsigned count1table_select;
		
        unsigned part2_length;
        unsigned sfb_lmax;
        unsigned address1;
        unsigned address2;
        unsigned address3;
        int quantizerStepSize;
        unsigned slen[4];
	} gr_info;
	
	typedef struct {
		int main_data_begin; /* unsigned -> int */
		unsigned private_bits;
		int resvDrain;
		unsigned scfsi[2][4];
		struct {
			struct {
				gr_info tt;
			} ch[2];
		} gr[2];
	} L3_side_info_t;
	
	typedef struct {
		double  l[2][2][21];
	} L3_psy_ratio_t;
	
	typedef struct {
        double  l[2][2][21];
	} L3_psy_xmin_t;
	
	typedef struct {
		int l[2][2][22];            /* [cb] */
		int s[2][2][13][3];         /* [window][cb] */
	} L3_scalefac_t;
	
	
	void  wave_open(config_t *config);
	int   wave_get(short buffer[2][samp_per_frame], void *config_in);
	void  wave_close(config_t *config);

	void L3_set_config_mpeg_defaults(mpeg_t *mpeg);
	int L3_find_bitrate_index(int bitr);
	int L3_find_samplerate_index(long freq);
	
	void L3_compress(config_t *config);

#ifdef __cplusplus
}
#endif
#endif