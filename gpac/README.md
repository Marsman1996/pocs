# poc12-AVC_DuplicateConfig-SEGV (CVE-2018-21015)
## Test Environment
Ubuntu 18.04, 64bit  
gpac (master 94ad872)

## How to trigger
`$ MP4Box -diso $POC`

## Reference
https://github.com/gpac/gpac/issues/1179
https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2018-21015

## Credits
Yanhao(unfuzzable123@gmail.com)  
Marsman1996(lqliuyuwei@outlook.com)

## Details
### Asan report

```
[iso file] Unknown box type m.ov
[iso file] Box "mvhd" is invalid in container m.ov
[iso file] Read Box type .... (0x00000001) has size 0 but is not at root/file level, skipping
/home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/avc_ext.c:847:35: runtime error: member access within null pointer of type 'struct GF_AVCConfig'
ASAN:DEADLYSIGNAL
=================================================================
==66133==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000003 (pc 0x7fcd396cfc95 bp 0x000000000000 sp 0x7fffa097b5d0 T0)
==66133==The signal is caused by a READ memory access.
==66133==Hint: address points to the zero page.
    #0 0x7fcd396cfc94 in AVC_DuplicateConfig /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/avc_ext.c:847
    #1 0x7fcd396d10f8 in merge_avc_config /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/avc_ext.c:897
    #2 0x7fcd396df0ca in AVC_RewriteESDescriptorEx /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/avc_ext.c:1039
    #3 0x7fcd3973468c in video_sample_entry_Read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_code_base.c:4291
    #4 0x7fcd397e5142 in gf_isom_box_read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1385
    #5 0x7fcd397e5142 in gf_isom_box_parse_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:199
    #6 0x7fcd397e5ec3 in gf_isom_box_array_read_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1277
    #7 0x7fcd397e5142 in gf_isom_box_read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1385
    #8 0x7fcd397e5142 in gf_isom_box_parse_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:199
    #9 0x7fcd397e5ec3 in gf_isom_box_array_read_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1277
    #10 0x7fcd3973b764 in stbl_Read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_code_base.c:5183
    #11 0x7fcd397e5142 in gf_isom_box_read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1385
    #12 0x7fcd397e5142 in gf_isom_box_parse_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:199
    #13 0x7fcd397e5ec3 in gf_isom_box_array_read_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1277
    #14 0x7fcd397300fc in minf_Read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_code_base.c:3513
    #15 0x7fcd397e5142 in gf_isom_box_read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1385
    #16 0x7fcd397e5142 in gf_isom_box_parse_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:199
    #17 0x7fcd397e5ec3 in gf_isom_box_array_read_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1277
    #18 0x7fcd3972c7f3 in mdia_Read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_code_base.c:3034
    #19 0x7fcd397e5142 in gf_isom_box_read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1385
    #20 0x7fcd397e5142 in gf_isom_box_parse_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:199
    #21 0x7fcd397e5ec3 in gf_isom_box_array_read_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1277
    #22 0x7fcd3974a187 in trak_Read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_code_base.c:6905
    #23 0x7fcd397e5142 in gf_isom_box_read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1385
    #24 0x7fcd397e5142 in gf_isom_box_parse_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:199
    #25 0x7fcd397e5ec3 in gf_isom_box_array_read_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1277
    #26 0x7fcd3971fdb7 in unkn_Read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_code_base.c:762
    #27 0x7fcd397e7363 in gf_isom_box_read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1385
    #28 0x7fcd397e7363 in gf_isom_box_parse_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:199
    #29 0x7fcd397e80c5 in gf_isom_parse_root_box /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:42
    #30 0x7fcd39814398 in gf_isom_parse_movie_boxes /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/isom_intern.c:206
    #31 0x7fcd398197a4 in gf_isom_open_file /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/isom_intern.c:615
    #32 0x56422e651046 in mp4boxMain /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/applications/mp4box/main.c:4539
    #33 0x7fcd366bcb96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)
    #34 0x56422e630199 in _start (/home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/build_asan/bin/gcc/MP4Box+0xac199)

AddressSanitizer can not provide additional info.
SUMMARY: AddressSanitizer: SEGV /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/avc_ext.c:847 in AVC_DuplicateConfig
==66133==ABORTING
```

### GDB report

```
Program received signal SIGSEGV, Segmentation fault.
AVC_DuplicateConfig (cfg=0x0)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/avc_ext.c:847
847		cfg_new->AVCLevelIndication = cfg->AVCLevelIndication;
(gdb) bt
#0  AVC_DuplicateConfig (cfg=0x0) at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/avc_ext.c:847
#1  0x00007ffff7856a5f in merge_avc_config (dst_cfg=dst_cfg@entry=0x5555557a8e00, src_cfg=<optimized out>)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/avc_ext.c:897
#2  0x00007ffff7859ae9 in AVC_RewriteESDescriptorEx (avc=avc@entry=0x5555557a8850, mdia=mdia@entry=0x0)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/avc_ext.c:1039
#3  0x00007ffff785a037 in AVC_RewriteESDescriptor (avc=avc@entry=0x5555557a8850)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/avc_ext.c:1067
#4  0x00007ffff786bd1c in video_sample_entry_Read (s=0x5555557a8850, bs=0x5555557a7f70)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_code_base.c:4291
#5  0x00007ffff7891fa7 in gf_isom_box_read (bs=0x5555557a7f70, a=0x5555557a8850)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1385
#6  gf_isom_box_parse_ex (outBox=0x7fffffff8af8, bs=0x5555557a7f70, parent_type=<optimized out>, is_root_box=<optimized out>)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:199
#7  0x00007ffff789254d in gf_isom_box_array_read_ex (parent=0x5555557a8800, bs=0x5555557a7f70, add_box=0x7ffff7865140 <stsd_AddBox>, 
    parent_type=1937011556) at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1277
#8  0x00007ffff7891fa7 in gf_isom_box_read (bs=0x5555557a7f70, a=0x5555557a8800)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1385
#9  gf_isom_box_parse_ex (outBox=0x7fffffff8bf8, bs=0x5555557a7f70, parent_type=<optimized out>, is_root_box=<optimized out>)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:199
#10 0x00007ffff789254d in gf_isom_box_array_read_ex (parent=parent@entry=0x5555557a8730, bs=0x5555557a7f70, 
    add_box=add_box@entry=0x7ffff7863750 <stbl_AddBox>, parent_type=parent_type@entry=0)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1277
#11 0x00007ffff7892837 in gf_isom_box_array_read (parent=parent@entry=0x5555557a8730, bs=<optimized out>, 
    add_box=add_box@entry=0x7ffff7863750 <stbl_AddBox>) at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:262
#12 0x00007ffff786d255 in stbl_Read (s=0x5555557a8730, bs=<optimized out>)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_code_base.c:5183
#13 0x00007ffff7891fa7 in gf_isom_box_read (bs=0x5555557a7f70, a=0x5555557a8730)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1385
#14 gf_isom_box_parse_ex (outBox=0x7fffffff8d18, bs=0x5555557a7f70, parent_type=<optimized out>, is_root_box=<optimized out>)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:199
#15 0x00007ffff789254d in gf_isom_box_array_read_ex (parent=parent@entry=0x5555557a8470, bs=0x5555557a7f70, 
    add_box=add_box@entry=0x7ffff7863450 <minf_AddBox>, parent_type=parent_type@entry=0)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1277
#16 0x00007ffff7892837 in gf_isom_box_array_read (parent=parent@entry=0x5555557a8470, bs=<optimized out>, 
    add_box=add_box@entry=0x7ffff7863450 <minf_AddBox>) at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:262
#17 0x00007ffff786acfb in minf_Read (s=0x5555557a8470, bs=<optimized out>)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_code_base.c:3513
#18 0x00007ffff7891fa7 in gf_isom_box_read (bs=0x5555557a7f70, a=0x5555557a8470)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1385
#19 gf_isom_box_parse_ex (outBox=0x7fffffff8e58, bs=0x5555557a7f70, parent_type=<optimized out>, is_root_box=<optimized out>)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:199
#20 0x00007ffff789254d in gf_isom_box_array_read_ex (parent=parent@entry=0x5555557a82c0, bs=0x5555557a7f70, 
    add_box=add_box@entry=0x7ffff7863330 <mdia_AddBox>, parent_type=parent_type@entry=0)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1277
#21 0x00007ffff7892837 in gf_isom_box_array_read (parent=parent@entry=0x5555557a82c0, bs=<optimized out>, 
    add_box=add_box@entry=0x7ffff7863330 <mdia_AddBox>) at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:262
#22 0x00007ffff786a090 in mdia_Read (s=0x5555557a82c0, bs=<optimized out>)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_code_base.c:3034
#23 0x00007ffff7891fa7 in gf_isom_box_read (bs=0x5555557a7f70, a=0x5555557a82c0)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1385
#24 gf_isom_box_parse_ex (outBox=0x7fffffff8f68, bs=0x5555557a7f70, parent_type=<optimized out>, is_root_box=<optimized out>)
---Type <return> to continue, or q <return> to quit---
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:199
#25 0x00007ffff789254d in gf_isom_box_array_read_ex (parent=parent@entry=0x5555557a8100, bs=0x5555557a7f70, 
    add_box=add_box@entry=0x7ffff7863ec0 <trak_AddBox>, parent_type=parent_type@entry=0)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1277
#26 0x00007ffff7892837 in gf_isom_box_array_read (parent=parent@entry=0x5555557a8100, bs=<optimized out>, 
    add_box=add_box@entry=0x7ffff7863ec0 <trak_AddBox>) at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:262
#27 0x00007ffff786fd1d in trak_Read (s=0x5555557a8100, bs=<optimized out>)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_code_base.c:6905
#28 0x00007ffff7891fa7 in gf_isom_box_read (bs=0x5555557a7f70, a=0x5555557a8100)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1385
#29 gf_isom_box_parse_ex (outBox=0x7fffffff90c8, bs=0x5555557a7f70, parent_type=<optimized out>, is_root_box=<optimized out>)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:199
#30 0x00007ffff789254d in gf_isom_box_array_read_ex (parent=parent@entry=0x5555557a7bf0, bs=bs@entry=0x5555557a7f70, 
    add_box=0x7ffff7891be0 <gf_isom_box_add_default>, parent_type=parent_type@entry=0)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1277
#31 0x00007ffff7892837 in gf_isom_box_array_read (parent=parent@entry=0x5555557a7bf0, bs=bs@entry=0x5555557a7f70, add_box=<optimized out>)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:262
#32 0x00007ffff7866a8a in unkn_Read (s=0x5555557a7bf0, bs=<optimized out>)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_code_base.c:762
#33 0x00007ffff7892bc9 in gf_isom_box_read (bs=0x5555557a6a60, a=0x5555557a7bf0)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1385
#34 gf_isom_box_parse_ex (outBox=outBox@entry=0x7fffffff9280, bs=bs@entry=0x5555557a6a60, is_root_box=is_root_box@entry=GF_TRUE, parent_type=0)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:199
#35 0x00007ffff7892fc5 in gf_isom_parse_root_box (outBox=outBox@entry=0x7fffffff9280, bs=0x5555557a6a60, 
    bytesExpected=bytesExpected@entry=0x7fffffff92d0, progressive_mode=progressive_mode@entry=GF_FALSE)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:42
#36 0x00007ffff789a20b in gf_isom_parse_movie_boxes (mov=mov@entry=0x5555557a68a0, bytesMissing=bytesMissing@entry=0x7fffffff92d0, 
    progressive_mode=progressive_mode@entry=GF_FALSE) at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/isom_intern.c:206
#37 0x00007ffff789b048 in gf_isom_parse_movie_boxes (progressive_mode=GF_FALSE, bytesMissing=0x7fffffff92d0, mov=0x5555557a68a0)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/isom_intern.c:194
#38 gf_isom_open_file (fileName=0x7fffffffe1a0 "../../poc12-SEGV", OpenMode=0, tmp_dir=0x0)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/isom_intern.c:615
#39 0x000055555556f3bd in mp4boxMain (argc=<optimized out>, argv=<optimized out>)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/applications/mp4box/main.c:4539
#40 0x00007ffff7331b97 in __libc_start_main (main=0x555555561e30 <main>, argc=3, argv=0x7fffffffdd98, init=<optimized out>, fini=<optimized out>, 
    rtld_fini=<optimized out>, stack_end=0x7fffffffdd88) at ../csu/libc-start.c:310
#41 0x0000555555561e6a in _start ()
```

# poc13-leak (CVE-2018-21016)
## Test Environment
Ubuntu 18.04, 64bit  
gpac (master 94ad872)

## How to trigger
`$ ./configure --extra-cflags="-fsanitize=address,undefined -g" --extra-ldflags="-fsanitize=address,undefined -ldl -g"`  
`$ MP4Box -diso $POC`

## Reference
https://github.com/gpac/gpac/issues/1183
https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2018-21016

## Credits
Yanhao(unfuzzable123@gmail.com)  
Marsman1996(lqliuyuwei@outlook.com)

## Details
### Asan report

```
=================================================================
==70416==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 40 byte(s) in 1 object(s) allocated from:
    #0 0x7f31574bab50 in __interceptor_malloc (/usr/lib/x86_64-linux-gnu/libasan.so.4+0xdeb50)
    #1 0x7f3153fe934a in dref_New /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_code_base.c:1012

SUMMARY: AddressSanitizer: 40 byte(s) leaked in 1 allocation(s).
```

# poc14-audio_sample_entry_AddBox-heapoverflow (CVE-2018-21017)
## Test Environment
Ubuntu 18.04, 64bit  
gpac (master 94ad872)

## How to trigger
`$ ./configure --extra-cflags="-fsanitize=address,undefined -g" --extra-ldflags="-fsanitize=address,undefined -ldl -g"`  
`$ MP4Box -diso $POC`

## Reference
https://github.com/gpac/gpac/issues/1180
https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2018-21017

## Credits
Yanhao(unfuzzable123@gmail.com)  
Marsman1996(lqliuyuwei@outlook.com)

## Details
### Asan report

```
==71438==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x603000000591 at pc 0x7ffa85321aff bp 0x7ffc13f5e4b0 sp 0x7ffc13f5e4a0
READ of size 1 at 0x603000000591 thread T0
    #0 0x7ffa85321afe in audio_sample_entry_AddBox /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_code_base.c:3934
    #1 0x7ffa853f002c in gf_isom_box_array_read_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1327
    #2 0x7ffa8533c83b in audio_sample_entry_Read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_code_base.c:3999
    #3 0x7ffa853ef142 in gf_isom_box_read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1385
    #4 0x7ffa853ef142 in gf_isom_box_parse_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:199
    #5 0x7ffa853efec3 in gf_isom_box_array_read_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1277
    #6 0x7ffa85329db7 in unkn_Read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_code_base.c:762
    #7 0x7ffa853ef142 in gf_isom_box_read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1385
    #8 0x7ffa853ef142 in gf_isom_box_parse_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:199
    #9 0x7ffa853efec3 in gf_isom_box_array_read_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1277
    #10 0x7ffa853ef142 in gf_isom_box_read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1385
    #11 0x7ffa853ef142 in gf_isom_box_parse_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:199
    #12 0x7ffa853efec3 in gf_isom_box_array_read_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1277
    #13 0x7ffa8533a0fc in minf_Read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_code_base.c:3513
    #14 0x7ffa853ef142 in gf_isom_box_read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1385
    #15 0x7ffa853ef142 in gf_isom_box_parse_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:199
    #16 0x7ffa853efec3 in gf_isom_box_array_read_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1277
    #17 0x7ffa853367f3 in mdia_Read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_code_base.c:3034
    #18 0x7ffa853ef142 in gf_isom_box_read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1385
    #19 0x7ffa853ef142 in gf_isom_box_parse_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:199
    #20 0x7ffa853efec3 in gf_isom_box_array_read_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1277
    #21 0x7ffa85354187 in trak_Read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_code_base.c:6905
    #22 0x7ffa853ef142 in gf_isom_box_read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1385
    #23 0x7ffa853ef142 in gf_isom_box_parse_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:199
    #24 0x7ffa853efec3 in gf_isom_box_array_read_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1277
    #25 0x7ffa85329db7 in unkn_Read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_code_base.c:762
    #26 0x7ffa853f1363 in gf_isom_box_read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:1385
    #27 0x7ffa853f1363 in gf_isom_box_parse_ex /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:199
    #28 0x7ffa853f20c5 in gf_isom_parse_root_box /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_funcs.c:42
    #29 0x7ffa8541e398 in gf_isom_parse_movie_boxes /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/isom_intern.c:206
    #30 0x7ffa854237a4 in gf_isom_open_file /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/isom_intern.c:615
    #31 0x55e7b46eb046 in mp4boxMain /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/applications/mp4box/main.c:4539
    #32 0x7ffa822c6b96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)
    #33 0x55e7b46ca199 in _start (/home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/build_asan/bin/gcc/MP4Box+0xac199)

0x603000000591 is located 0 bytes to the right of 17-byte region [0x603000000580,0x603000000591)
allocated by thread T0 here:
    #0 0x7ffa887fcb50 in __interceptor_malloc (/usr/lib/x86_64-linux-gnu/libasan.so.4+0xdeb50)
    #1 0x7ffa85329a80 in unkn_Read /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_code_base.c:742

SUMMARY: AddressSanitizer: heap-buffer-overflow /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/box_code_base.c:3934 in audio_sample_entry_AddBox
Shadow bytes around the buggy address:
  0x0c067fff8060: fa fa 00 00 02 fa fa fa 00 00 05 fa fa fa 00 00
  0x0c067fff8070: 04 fa fa fa 00 00 00 01 fa fa 00 00 06 fa fa fa
  0x0c067fff8080: 00 00 01 fa fa fa 00 00 02 fa fa fa 00 00 00 01
  0x0c067fff8090: fa fa 00 00 05 fa fa fa 00 00 04 fa fa fa 00 00
  0x0c067fff80a0: 02 fa fa fa 00 00 04 fa fa fa 00 00 00 00 fa fa
=>0x0c067fff80b0: 00 00[01]fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c067fff80c0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c067fff80d0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c067fff80e0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c067fff80f0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c067fff8100: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
==71438==ABORTING
```

### GDB report

```
malloc_consolidate(): invalid chunk size

Program received signal SIGABRT, Aborted.
__GI_raise (sig=sig@entry=6) at ../sysdeps/unix/sysv/linux/raise.c:51
51	../sysdeps/unix/sysv/linux/raise.c: No such file or directory.
(gdb) bt
#0  __GI_raise (sig=sig@entry=6) at ../sysdeps/unix/sysv/linux/raise.c:51
#1  0x00007ffff7350801 in __GI_abort () at abort.c:79
#2  0x00007ffff7399897 in __libc_message (action=action@entry=do_abort, fmt=fmt@entry=0x7ffff74c6b9a "%s\n") at ../sysdeps/posix/libc_fatal.c:181
#3  0x00007ffff73a090a in malloc_printerr (str=str@entry=0x7ffff74c83f0 "malloc_consolidate(): invalid chunk size") at malloc.c:5350
#4  0x00007ffff73a0bae in malloc_consolidate (av=av@entry=0x7ffff76fbc40 <main_arena>) at malloc.c:4441
#5  0x00007ffff73a47d8 in _int_malloc (av=av@entry=0x7ffff76fbc40 <main_arena>, bytes=bytes@entry=4096) at malloc.c:3703
#6  0x00007ffff73a70fc in __GI___libc_malloc (bytes=4096) at malloc.c:3057
#7  0x00007ffff738e18c in __GI__IO_file_doallocate (fp=0x5555557a6260) at filedoalloc.c:101
#8  0x00007ffff739e379 in __GI__IO_doallocbuf (fp=fp@entry=0x5555557a6260) at genops.c:365
#9  0x00007ffff739ad23 in _IO_new_file_seekoff (fp=0x5555557a6260, offset=0, dir=2, mode=<optimized out>) at fileops.c:960
#10 0x00007ffff7398dd9 in fseeko (fp=fp@entry=0x5555557a6260, offset=offset@entry=0, whence=whence@entry=2) at fseeko.c:36
#11 0x00007ffff77527c9 in gf_fseek (fp=fp@entry=0x5555557a6260, offset=offset@entry=0, whence=whence@entry=2)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/utils/os_file.c:756
#12 0x00007ffff7753323 in gf_bs_from_file (f=0x5555557a6260, mode=mode@entry=0) at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/utils/bitstream.c:179
#13 0x00007ffff7894173 in gf_isom_fdm_new (sPath=<optimized out>, mode=<optimized out>) at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/data_map.c:453
#14 0x00007ffff7894400 in gf_isom_datamap_new (location=<optimized out>, location@entry=0x7fffffffe197 "../../poc14-heapoverflow", parentPath=parentPath@entry=0x0, 
    mode=mode@entry=1 '\001', outDataMap=outDataMap@entry=0x5555557a68b0) at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/data_map.c:185
#15 0x00007ffff789cf66 in gf_isom_open_progressive (fileName=<optimized out>, start_range=0, end_range=0, the_file=0x5555557a5738 <file>, BytesMissing=0x7fffffff9390)
    at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/src/isomedia/isom_read.c:367
#16 0x000055555556f48b in mp4boxMain (argc=<optimized out>, argv=<optimized out>) at /home/ubuntu/Desktop/crashana/gpac/gpac-94ad872/applications/mp4box/main.c:4542
#17 0x00007ffff7331b97 in __libc_start_main (main=0x555555561e30 <main>, argc=3, argv=0x7fffffffdd98, init=<optimized out>, fini=<optimized out>, rtld_fini=<optimized out>, 
    stack_end=0x7fffffffdd88) at ../csu/libc-start.c:310
#18 0x0000555555561e6a in _start ()
```