;
; CIS 668 driver 		668_lib.inc
;

.define	cis_frame_flag		0	; new flame flag
.define	cis_line_flag		1	; new line flag
.define	cis_line_count		2	; read line count
.define	cis_buffer_point		3	; read line buffer point
.define	cis_frame_count		4	; read frame count
.define	cis_error_flag		0x100	; read cis error flag

CMOS_CIF==1	; 1: CIF, 0: QCIF

.if CMOS_CIF
.define	ini_cis_x_byte		240
.define	ini_cis_y_byte		240
.else
.define	ini_cis_x_byte		120
.define	ini_cis_y_byte		120
.endif

.define	cis_vsync_dir		pcdir.3
.define	cis_vsync_en		pcen.3
.define	cis_vsync_id		pcid
.define	cis_vsync_ph		pcph.3
.define	cis_vsync			r3.3
.define	cis_vsync_pif		pcpif.3
.define	cis_vsync_icr		pcicr.0
.define	cis_vsync_pie		pcpie.3
