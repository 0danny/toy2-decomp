#if 0

// The game has a statically linked single threaded CRT
// The names of a lot of these are probably wrong and will need to be changed over time

// FUNCTION: TOY2 0x0047C7F0
// ___setargv

// FUNCTION: TOY2 0x004B2AA0
// __mkgmtime

// FUNCTION: TOY2 0x004CEB00
// msvc_float_method

// FUNCTION: TOY2 0x004CEB18
// __cfltcvt_init_4

// FUNCTION: TOY2 0x004CEB60
// __ftol

// FUNCTION: TOY2 0x004CEB87
// _qsort

// FUNCTION: TOY2 0x004CECDB
// _shortsort

// FUNCTION: TOY2 0x004CED29
// _swap

// FUNCTION: TOY2 0x004CED60
// _strncpy

// FUNCTION: TOY2 0x004CEE5E
// _free

// FUNCTION: TOY2 0x004CEE8D
// _sprintf

// FUNCTION: TOY2 0x004CEEDF
// __onexit

// FUNCTION: TOY2 0x004CEF4C
// _atexit

// FUNCTION: TOY2 0x004CEF5E
// ___onexitinit

// FUNCTION: TOY2 0x004CEF8D
// _atol

// FUNCTION: TOY2 0x004CF018
// _atoi

// FUNCTION: TOY2 0x004CF023
// __atoi64

// FUNCTION: TOY2 0x004CF0DD
// _fscanf

// FUNCTION: TOY2 0x004CF0F3
// _fgetc

// FUNCTION: TOY2 0x004CF118
// __strlwr

// FUNCTION: TOY2 0x004CF1B6
// _fclose

// FUNCTION: TOY2 0x004CF20C
// __fsopen

// FUNCTION: TOY2 0x004CF22C
// _fopen

// FUNCTION: TOY2 0x004CF23F
// _strtok

// FUNCTION: TOY2 0x004CF2DB
// _vsprintf

// FUNCTION: TOY2 0x004CF32C
// __cinit

// FUNCTION: TOY2 0x004CF359
// _exit

// FUNCTION: TOY2 0x004CF36A
// __exit

// FUNCTION: TOY2 0x004CF37B
// __cexit

// FUNCTION: TOY2 0x004CF38A
// __c_exit

// FUNCTION: TOY2 0x004CF399
// _doexit

// FUNCTION: TOY2 0x004CF432
// __initterm

// FUNCTION: TOY2 0x004CF44C
// _malloc

// FUNCTION: TOY2 0x004CF45E
// __nh_malloc

// FUNCTION: TOY2 0x004CF48A
// __heap_alloc

// FUNCTION: TOY2 0x004CF4C0
// _strstr

// FUNCTION: TOY2 0x004CF54A
// _rand

// FUNCTION: TOY2 0x004CF568
// _strerror

// FUNCTION: TOY2 0x004CF594
// _fread

// FUNCTION: TOY2 0x004CF680
// __alloca_probe

// FUNCTION: TOY2 0x004CF6AF
// _fwrite

// FUNCTION: TOY2 0x004CF7B9
// _abs

// FUNCTION: TOY2 0x004CF7D0
// __CIpow

// FUNCTION: TOY2 0x004CF7E9
// _pow

// FUNCTION: TOY2 0x004CF7F2
// msvc_float_method8

// FUNCTION: TOY2 0x004CF9C5
// msvc_float_method7

// FUNCTION: TOY2 0x004CF9ED
// _fputc

// FUNCTION: TOY2 0x004CFA21
// _fprintf

// FUNCTION: TOY2 0x004CFA53
// _remove

// FUNCTION: TOY2 0x004CFA88
// _ftell

// FUNCTION: TOY2 0x004CFBE0
// _fseek

// FUNCTION: TOY2 0x004CFD62
// __amsg_exit

// FUNCTION: TOY2 0x004CFD87
// _fast_error_exit

// FUNCTION: TOY2 0x004CFDAB
// __statusfp

// FUNCTION: TOY2 0x004CFDBE
// __clearfp

// FUNCTION: TOY2 0x004CFDD2
// __control87

// FUNCTION: TOY2 0x004CFE07
// __controlfp

// FUNCTION: TOY2 0x004CFE1D
// __fpreset

// FUNCTION: TOY2 0x004CFE47
// __abstract_cw

// FUNCTION: TOY2 0x004CFED9
// __hw_cw

// FUNCTION: TOY2 0x004CFF62
// __abstract_sw

// FUNCTION: TOY2 0x004CFFA0
// __CIacos

// FUNCTION: TOY2 0x004CFFB4
// _acos

// FUNCTION: TOY2 0x004CFFBD
// msvc_float_method9

// FUNCTION: TOY2 0x004D0070
// __alldiv

// FUNCTION: TOY2 0x004D0120
// __allmul

// FUNCTION: TOY2 0x004D0154
// _operator_new

// FUNCTION: TOY2 0x004D0162
// _operator_delete

// FUNCTION: TOY2 0x004D0170
// _strrchr

// FUNCTION: TOY2 0x004D0197
// _getenv

// FUNCTION: TOY2 0x004D0214
// __setdefaultprecision

// FUNCTION: TOY2 0x004D0226
// __ms_p5_test_fdiv

// FUNCTION: TOY2 0x004D0264
// __ms_p5_mp_test_fdiv

// FUNCTION: TOY2 0x004D028D
// __forcdecpt

// FUNCTION: TOY2 0x004D02E7
// __cropzeros

// FUNCTION: TOY2 0x004D0335
// __positive

// FUNCTION: TOY2 0x004D034D
// __fassign

// FUNCTION: TOY2 0x004D038B
// __cftoe

// FUNCTION: TOY2 0x004D048F
// __cftof

// FUNCTION: TOY2 0x004D056D
// __cftog

// FUNCTION: TOY2 0x004D0608
// __cftoe_g

// FUNCTION: TOY2 0x004D062F
// __cftof_g

// FUNCTION: TOY2 0x004D0652
// __cfltcvt

// FUNCTION: TOY2 0x004D06A3
// __shift

// FUNCTION: TOY2 0x004D06C8
// __heap_init

// FUNCTION: TOY2 0x004D0704
// __heap_term

// FUNCTION: TOY2 0x004D077F
// __set_sbh_threshold

// FUNCTION: TOY2 0x004D0796
// ___sbh_heap_init

// FUNCTION: TOY2 0x004D07D4
// ___sbh_find_block

// FUNCTION: TOY2 0x004D07FF
// ___sbh_free_block

// FUNCTION: TOY2 0x004D0B2A
// ___sbh_alloc_block

// FUNCTION: TOY2 0x004D0E33
// ___sbh_alloc_new_region

// FUNCTION: TOY2 0x004D0EE4
// ___sbh_alloc_new_group

// FUNCTION: TOY2 0x004D0FDF
// ___sbh_resize_block

// FUNCTION: TOY2 0x004D13A6
// ___sbh_heap_check

// FUNCTION: TOY2 0x004D16D5
// __flsbuf

// FUNCTION: TOY2 0x004D17EA
// __output

// FUNCTION: TOY2 0x004D1F2B
// _write_char

// FUNCTION: TOY2 0x004D1F60
// _write_multi_char

// FUNCTION: TOY2 0x004D1F91
// _write_string

// FUNCTION: TOY2 0x004D1FC9
// _get_int_arg

// FUNCTION: TOY2 0x004D1FD6
// _get_int64_arg

// FUNCTION: TOY2 0x004D1FE6
// _get_short_arg

// FUNCTION: TOY2 0x004D1FF4
// _realloc

// FUNCTION: TOY2 0x004D2114
// __msize

// FUNCTION: TOY2 0x004D213D
// __isctype

// FUNCTION: TOY2 0x004D21B2
// __input

// FUNCTION: TOY2 0x004D2BD7
// __hextodec

// FUNCTION: TOY2 0x004D2C0E
// _fgetc_0

// FUNCTION: TOY2 0x004D2C28
// __un_inc

// FUNCTION: TOY2 0x004D2C3F
// __whiteout

// FUNCTION: TOY2 0x004D2C63
// __filbuf

// FUNCTION: TOY2 0x004D2D40
// _strcpy

// FUNCTION: TOY2 0x004D2D50
// _strcat

// FUNCTION: TOY2 0x004D2E30
// ___crtLCMapStringA

// FUNCTION: TOY2 0x004D3054
// _strncnt

// FUNCTION: TOY2 0x004D307F
// __close

// FUNCTION: TOY2 0x004D3132
// __freebuf

// FUNCTION: TOY2 0x004D315D
// _fflush

// FUNCTION: TOY2 0x004D3198
// __flush

// FUNCTION: TOY2 0x004D31F4
// msvc_float_method5

// FUNCTION: TOY2 0x004D31FD
// _flsall

// FUNCTION: TOY2 0x004D326A
// __openfile

// FUNCTION: TOY2 0x004D33DA
// __getstream

// FUNCTION: TOY2 0x004D3468
// __callnewh

// FUNCTION: TOY2 0x004D34A0
// _strchr

// FUNCTION: TOY2 0x004D355C
// __read

// FUNCTION: TOY2 0x004D3760
// _memcpy

// FUNCTION: TOY2 0x004D3A95
// __write

// FUNCTION: TOY2 0x004D3C61
// __fFEXP

// FUNCTION: TOY2 0x004D3D37
// _zerotoxdone

// FUNCTION: TOY2 0x004D3D82
// _expbigret

// FUNCTION: TOY2 0x004D3DA1
// _rtforexpinf

// FUNCTION: TOY2 0x004D3DAE
// __ffexpm1

// FUNCTION: TOY2 0x004D3DF1
// _isintTOS

// FUNCTION: TOY2 0x004D3E16
// _isintTOSret

// FUNCTION: TOY2 0x004D3E17
// _notanint

// FUNCTION: TOY2 0x004D3E1E
// _evenint

// FUNCTION: TOY2 0x004D3E25
// _usepowhlp

// FUNCTION: TOY2 0x004D4030
// __startTwoArgErrorHandling

// FUNCTION: TOY2 0x004D4047
// __startOneArgErrorHandling

// FUNCTION: TOY2 0x004D4090
// __twoToTOS

// FUNCTION: TOY2 0x004D40A5
// __load_CW

// FUNCTION: TOY2 0x004D40BC
// __convertTOStoQNaN

// FUNCTION: TOY2 0x004D40D5
// __fload_withFB

// FUNCTION: TOY2 0x004D4118
// __checkTOS_withFB

// FUNCTION: TOY2 0x004D4165
// __check_overflow_exit

// FUNCTION: TOY2 0x004D4179
// __check_range_exit

// FUNCTION: TOY2 0x004D421C
// __powhlp

// FUNCTION: TOY2 0x004D434C
// __d_inttype

// FUNCTION: TOY2 0x004D43B1
// __stbuf

// FUNCTION: TOY2 0x004D443E
// __ftbuf

// FUNCTION: TOY2 0x004D447B
// __dosmaperr

// FUNCTION: TOY2 0x004D44E2
// __ioinit

// FUNCTION: TOY2 0x004D46B0
// __lseek

// FUNCTION: TOY2 0x004D474A
// __XcptFilter

// FUNCTION: TOY2 0x004D488B
// _xcptlookup

// FUNCTION: TOY2 0x004D48CE
// __wincmdln

// FUNCTION: TOY2 0x004D4926
// __setenvp

// FUNCTION: TOY2 0x004D49DF
// __setargv

// FUNCTION: TOY2 0x004D4A78
// _parse_cmdline

// FUNCTION: TOY2 0x004D4C2C
// ___crtGetEnvironmentStringsA

// FUNCTION: TOY2 0x004D4D60
// __global_unwind2

// FUNCTION: TOY2 0x004D4D80
// __unwind_handler

// FUNCTION: TOY2 0x004D4DA2
// __local_unwind2

// FUNCTION: TOY2 0x004D4E0A
// __abnormal_termination

// FUNCTION: TOY2 0x004D4E2D
// __NLG_Notify1

// FUNCTION: TOY2 0x004D4E36
// __NLG_Notify

// FUNCTION: TOY2 0x004D4E58
// __except_handler3

// FUNCTION: TOY2 0x004D4F15
// __seh_longjmp_unwind@4

// FUNCTION: TOY2 0x004D4F30
// __FF_MSGBANNER

// FUNCTION: TOY2 0x004D4F69
// __NMSG_WRITE

// FUNCTION: TOY2 0x004D50ED
// __mbsnbicoll

// FUNCTION: TOY2 0x004D5130
// _strlen

// FUNCTION: TOY2 0x004D51AB
// ___wtomb_environ

// FUNCTION: TOY2 0x004D5219
// __tolower

// FUNCTION: TOY2 0x004D5221
// _tolower

// FUNCTION: TOY2 0x004D52EC
// __ZeroTail

// FUNCTION: TOY2 0x004D5335
// __IncMan

// FUNCTION: TOY2 0x004D538B
// __RoundMan

// FUNCTION: TOY2 0x004D5417
// __CopyMan

// FUNCTION: TOY2 0x004D5432
// __FillZeroMan

// FUNCTION: TOY2 0x004D543E
// __IsZeroMan

// FUNCTION: TOY2 0x004D5459
// __ShrMan

// FUNCTION: TOY2 0x004D54E6
// __ld12cvt

// FUNCTION: TOY2 0x004D5652
// msvc_float_method6

// FUNCTION: TOY2 0x004D5668
// msvc_float_method3

// FUNCTION: TOY2 0x004D567E
// __ld12told

// FUNCTION: TOY2 0x004D56F8
// msvc_float_method4

// FUNCTION: TOY2 0x004D5725
// __atoldbl

// FUNCTION: TOY2 0x004D5753
// msvc_float_method2

// FUNCTION: TOY2 0x004D5780
// __fptostr

// FUNCTION: TOY2 0x004D57F7
// __fltout

// FUNCTION: TOY2 0x004D585B
// ___dtold

// FUNCTION: TOY2 0x004D5920
// _memset

// FUNCTION: TOY2 0x004D5980
// _memcpy_0

// FUNCTION: TOY2 0x004D5CB5
// __fptrap

// FUNCTION: TOY2 0x004D5CBE
// __getbuf

// FUNCTION: TOY2 0x004D5D02
// __isatty

// FUNCTION: TOY2 0x004D5D28
// ___initstdio

// FUNCTION: TOY2 0x004D5DCD
// ___endstdio

// FUNCTION: TOY2 0x004D5DE1
// _wctomb

// FUNCTION: TOY2 0x004D5E50
// __aulldiv

// FUNCTION: TOY2 0x004D5EC0
// __aullrem

// FUNCTION: TOY2 0x004D5F35
// ___crtGetStringTypeA

// FUNCTION: TOY2 0x004D607E
// _mbtowc

// FUNCTION: TOY2 0x004D6146
// _isalpha

// FUNCTION: TOY2 0x004D6174
// _isupper

// FUNCTION: TOY2 0x004D619C
// _islower

// FUNCTION: TOY2 0x004D61C4
// _isdigit

// FUNCTION: TOY2 0x004D61EC
// _isxdigit

// FUNCTION: TOY2 0x004D6219
// _isspace

// FUNCTION: TOY2 0x004D6241
// _ispunct

// FUNCTION: TOY2 0x004D6269
// _isalnum

// FUNCTION: TOY2 0x004D6297
// _isprint

// FUNCTION: TOY2 0x004D62C5
// _isgraph

// FUNCTION: TOY2 0x004D62F3
// _iscntrl

// FUNCTION: TOY2 0x004D631B
// ___isascii

// FUNCTION: TOY2 0x004D6328
// ___toascii

// FUNCTION: TOY2 0x004D6330
// ___iscsymf

// FUNCTION: TOY2 0x004D636D
// ___iscsym

// FUNCTION: TOY2 0x004D63B0
// __allshl

// FUNCTION: TOY2 0x004D63CF
// _ungetc

// FUNCTION: TOY2 0x004D643D
// __alloc_osfhnd

// FUNCTION: TOY2 0x004D64D2
// __set_osfhnd

// FUNCTION: TOY2 0x004D6549
// __free_osfhnd

// FUNCTION: TOY2 0x004D65C3
// __get_osfhandle

// FUNCTION: TOY2 0x004D6600
// __open_osfhandle

// FUNCTION: TOY2 0x004D669D
// __commit

// FUNCTION: TOY2 0x004D670B
// __sopen

// FUNCTION: TOY2 0x004D775B
// __87except

// FUNCTION: TOY2 0x004D782B
// __set_exp

// FUNCTION: TOY2 0x004D7894
// __set_bexp

// FUNCTION: TOY2 0x004D78B8
// __sptype

// FUNCTION: TOY2 0x004D7912
// __decomp

// FUNCTION: TOY2 0x004D79D3
// __frnd

// FUNCTION: TOY2 0x004D79E5
// __copysign

// FUNCTION: TOY2 0x004D7A06
// __chgsign

// FUNCTION: TOY2 0x004D7A2B
// __scalb

// FUNCTION: TOY2 0x004D7A41
// __logb

// FUNCTION: TOY2 0x004D7B28
// __nextafter

// FUNCTION: TOY2 0x004D7DBD
// __finite

// FUNCTION: TOY2 0x004D7DD1
// __isnan

// FUNCTION: TOY2 0x004D7DFF
// __fpclass

// FUNCTION: TOY2 0x004D7EA2
// __ismbbkprint

// FUNCTION: TOY2 0x004D7EB3
// __ismbbkpunct

// FUNCTION: TOY2 0x004D7EC4
// __ismbbalnum

// FUNCTION: TOY2 0x004D7ED8
// __ismbbalpha

// FUNCTION: TOY2 0x004D7EEC
// __ismbbgraph

// FUNCTION: TOY2 0x004D7F00
// __ismbbprint

// FUNCTION: TOY2 0x004D7F14
// __ismbbpunct

// FUNCTION: TOY2 0x004D7F25
// __ismbblead

// FUNCTION: TOY2 0x004D7F36
// __ismbbtrail

// FUNCTION: TOY2 0x004D7F47
// __ismbbkana

// FUNCTION: TOY2 0x004D7F6E
// _x_ismbbtype

// FUNCTION: TOY2 0x004D7F9F
// __setmbcp

// FUNCTION: TOY2 0x004D8138
// _getSystemCP

// FUNCTION: TOY2 0x004D8182
// _CPtoLCID

// FUNCTION: TOY2 0x004D81B5
// _setSBCS

// FUNCTION: TOY2 0x004D81DE
// _setSBUpLow

// FUNCTION: TOY2 0x004D8363
// __getmbcp

// FUNCTION: TOY2 0x004D8373
// ___initmbctable

// FUNCTION: TOY2 0x004D838F
// ___crtMessageBoxA

// FUNCTION: TOY2 0x004D8418
// ___crtCompareStringA

// FUNCTION: TOY2 0x004D8695
// _strncnt_0

// FUNCTION: TOY2 0x004D86C0
// ___crtsetenv

// FUNCTION: TOY2 0x004D8847
// _findenv

// FUNCTION: TOY2 0x004D889F
// _copy_environ

// FUNCTION: TOY2 0x004D8906
// ___addl

// FUNCTION: TOY2 0x004D8927
// ___add_12

// FUNCTION: TOY2 0x004D8985
// ___shl_12

// FUNCTION: TOY2 0x004D89B3
// ___shr_12

// FUNCTION: TOY2 0x004D89E0
// ___mtold12

// FUNCTION: TOY2 0x004D8AA7
// ___strgtold12

// FUNCTION: TOY2 0x004D8F78
// ___STRINGTOLD

// FUNCTION: TOY2 0x004D8FB4
// _$I10_OUTPUT

// FUNCTION: TOY2 0x004D9247
// _calloc

// FUNCTION: TOY2 0x004D92C4
// __fcloseall

// FUNCTION: TOY2 0x004D931C
// __chsize

// FUNCTION: TOY2 0x004D9462
// __handle_qnan1

// FUNCTION: TOY2 0x004D94B5
// __handle_qnan2

// FUNCTION: TOY2 0x004D9514
// __except1

// FUNCTION: TOY2 0x004D95AC
// __except2

// FUNCTION: TOY2 0x004D9651
// __raise_exc

// FUNCTION: TOY2 0x004D9904
// __handle_exc

// FUNCTION: TOY2 0x004D9B1B
// __umatherr

// FUNCTION: TOY2 0x004D9BA3
// __set_errno

// FUNCTION: TOY2 0x004D9BC9
// __get_fname

// FUNCTION: TOY2 0x004D9BEE
// __errcode

// FUNCTION: TOY2 0x004D9C1B
// msvc_method_1

// FUNCTION: TOY2 0x004D9C1E
// __statfp

// FUNCTION: TOY2 0x004D9C2C
// __clrfp

// FUNCTION: TOY2 0x004D9C3B
// __ctrlfp

// FUNCTION: TOY2 0x004D9C5E
// __set_statfp

// FUNCTION: TOY2 0x004D9CB4
// _ldexp

// FUNCTION: TOY2 0x004D9E9A
// __mbschr

// FUNCTION: TOY2 0x004D9F0D
// __strdup

// FUNCTION: TOY2 0x004D9F38
// ___ld12mul

// FUNCTION: TOY2 0x004DA158
// ___multtenpow12

// FUNCTION: TOY2 0x004DA1D4
// __setmode

// FUNCTION: TOY2 0x004DBBB0
// __strcmpi

// FUNCTION: TOY2 0x004DBC3C
// _atof

// FUNCTION: TOY2 0x004DBC89
// _printf

// FUNCTION: TOY2 0x004DBCBA
// __fltin

#endif