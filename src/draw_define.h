

#ifndef FIEND_DRAW_DEFINE
#define FIEND_DRAW_DEFINE


/// the draw modes /////////

#define FIEND_DRAW_MODE_FLAT 0
#define FIEND_DRAW_MODE_TRANS 1
#define FIEND_DRAW_MODE_ADDITIVE 2


//////// Rotation draw loop defines ///////////////

#define ROTATE_DRAW_LOOP_TRANS16				\
	for(i=min_y;i<max_y;i++)					\
	{											\
		max_x = fixtoi(max_x_buffer[i]);		\
		min_x = fixtoi(min_x_buffer[i]);		\
												\
														\
		u = min_u_buffer[i];							\
		v = min_v_buffer[i];							\
														\
		j = max_x - min_x;								\
														\
		src_buffer = (short*)src->line[0];				\
		src_buffer += fixtoi(v)*src->w + fixtoi(u);			\
		src_add_buffer = src_add;								\
																\
		while(j){						\
			if(*src_buffer!= MASK_COLOR_16)								\
					*dest_buffer = *src_buffer;								\
																	\
			src_buffer+=*src_add_buffer;							\
																	\
			dest_buffer++;												\
			src_add_buffer++;											\
			j--;}						\
																		\
		if(i+1<max_y)														\
			dest_buffer +=(dest->w - max_x)+fixtoi(min_x_buffer[i+1]);	\
	}	

#define ROTATE_DRAW_LOOP_TRANS15				\
	for(i=min_y;i<max_y;i++)					\
	{											\
		max_x = fixtoi(max_x_buffer[i]);		\
		min_x = fixtoi(min_x_buffer[i]);		\
												\
		u = min_u_buffer[i];							\
		v = min_v_buffer[i];							\
														\
		j = max_x - min_x;								\
														\
		src_buffer = (short*)src->line[0];				\
		src_buffer += fixtoi(v)*src->w + fixtoi(u);			\
		src_add_buffer = src_add;								\
																\
		while(j){						\
																\
			if(*src_buffer!= MASK_COLOR_15)								\
				*dest_buffer = *src_buffer;								\
																	\
			src_buffer+=*src_add_buffer;							\
																	\
			dest_buffer++;												\
			src_add_buffer++;											\
			j--;}						\
																		\
		if(i<max_y)														\
			dest_buffer +=(dest->w - max_x)+fixtoi(min_x_buffer[i+1]);	\
}

#define ROTATE_DRAW_LOOP_FLAT16				\
	for(i=min_y;i<max_y;i++)					\
	{											\
		max_x = fixtoi(max_x_buffer[i]);		\
		min_x = fixtoi(min_x_buffer[i]);		\
												\
		u = min_u_buffer[i];							\
		v = min_v_buffer[i];							\
														\
		j = max_x - min_x;								\
														\
		src_buffer = (short*)src->line[0];				\
		src_buffer += fixtoi(v)*src->w + fixtoi(u);			\
		src_add_buffer = src_add;								\
																\
		while(j){						\
																\
			*dest_buffer = *src_buffer;								\
																	\
			src_buffer+=*src_add_buffer;							\
																	\
			dest_buffer++;												\
			src_add_buffer++;											\
			j--;}						\
																		\
		if(i+1<max_y)														\
			dest_buffer +=(dest->w - max_x)+fixtoi(min_x_buffer[i+1]);	\
}	


#define ROTATE_DRAW_LOOP_ADDITIVE16				\
	for(i=min_y;i<max_y;i++)					\
	{											\
		max_x = fixtoi(max_x_buffer[i]);		\
		min_x = fixtoi(min_x_buffer[i]);		\
												\
													\
		u = min_u_buffer[i];							\
		v = min_v_buffer[i];							\
														\
		j = max_x - min_x;								\
														\
		src_buffer = (short*)src->line[0];				\
		src_buffer += fixtoi(v)*src->w + fixtoi(u);			\
		src_add_buffer = src_add;								\
																\
		while(j){								\
			d_pix=0;							\
			temp_pix = (*src_buffer & RED_MASK16) + (*dest_buffer & RED_MASK16);	\
			if(temp_pix > RED_MASK16) temp_pix = RED_MASK16;							\
			d_pix |= temp_pix;															\
																						\
			temp_pix = (*src_buffer & GREEN_MASK16) + (*dest_buffer & GREEN_MASK16);	\
			if(temp_pix > GREEN_MASK16)temp_pix = GREEN_MASK16;							\
			d_pix |= temp_pix;															\
																						\
			temp_pix = (*src_buffer & BLUE_MASK16) + (*dest_buffer & BLUE_MASK16);		\
			if(temp_pix > BLUE_MASK16) temp_pix = BLUE_MASK16;							\
			d_pix |= temp_pix;											\
			*dest_buffer = (unsigned short)d_pix;					\
			src_buffer+=*src_add_buffer;							\
																	\
			dest_buffer++;												\
			src_add_buffer++;											\
			j--;}						\
																		\
		if(i<max_y)														\
			dest_buffer +=(dest->w - max_x)+fixtoi(min_x_buffer[i+1]);	\
}

	
#define ROTATE_DRAW_LOOP_ADDITIVE15				\
	for(i=min_y;i<max_y;i++)					\
	{											\
		max_x = fixtoi(max_x_buffer[i]);		\
		min_x = fixtoi(min_x_buffer[i]);		\
												\
														\
		u = min_u_buffer[i];							\
		v = min_v_buffer[i];							\
														\
		j = max_x - min_x;								\
														\
		src_buffer = (short*)src->line[0];				\
		src_buffer += fixtoi(v)*src->w + fixtoi(u);			\
		src_add_buffer = src_add;								\
																\
		while(j){								\
			d_pix=0;							\
			temp_pix = (*src_buffer & RED_MASK15) + (*dest_buffer & RED_MASK15);	\
			if(temp_pix > RED_MASK15) temp_pix = RED_MASK15;							\
			d_pix |= temp_pix;															\
			temp_pix = (*src_buffer & GREEN_MASK15) + (*dest_buffer & GREEN_MASK15);	\
			if(temp_pix > GREEN_MASK15)temp_pix = GREEN_MASK15;							\
			d_pix |= temp_pix;															\
			temp_pix = (*src_buffer & BLUE_MASK15) + (*dest_buffer & BLUE_MASK15);		\
			if(temp_pix > BLUE_MASK15) temp_pix = BLUE_MASK15;							\
			d_pix |= temp_pix;											\
			*dest_buffer = (unsigned short)d_pix;					\
														\
			src_buffer+=*src_add_buffer;							\
																	\
			dest_buffer++;												\
			src_add_buffer++;											\
			j--;}						\
																		\
		if(i<max_y)														\
			dest_buffer +=(dest->w - max_x)+fixtoi(min_x_buffer[i+1]);	\
}	


#endif	