/* 
   This file contains global version information
   about the pipeline. Please note that it lists
   changes in the INTERNAL and EXTERNAL (THELI) 
   pipelines.

   18.03.2005:
   Increase of the minor version number:
   significant changes in the absolute and 
   relative photometric modules

   09.04.2005:
   Increase of the minor version number:
   several small bug fixes and cosmetic changes.
   This version will be used to create reference
   reductions for later regression tests.

   02.05.2005:
   Increase of the minor version number:
   - major changes to 'mefsplit' to allow for more
     general transformations of input images
   - bug fix in ldacpaste
   - scripts for the generation of control plots
     for corrected absolute zeropoints
   - chips.cat4 contains a table SOLPHOTOM containing
     info on the photometric solutions finally
     selected.
   - changes to fitsort to distinguish 'empty' from
     'non existing' keywords
   - changes to create_astrometrix_astrom_run.sh so
     that the global solution is estimated simultaneously
     for all runs
   - I rewrote several scripts to use imstats (IMCAT) 
     instead of immode (Flips)
   - changes to the album program to allow a binning of
     input images
   
   27.06.2005:
   Increase of the minor version number:   
   - major bug fix in imcombflat: The sorting of the array for
     rejecting low/high pixels in the coaddition process was
     done at the wrong place.
   - bug fix in swarp: the total size of the output image was
     not calculated correctly
   - new program mefcreate to create MEF files from splitted
     mosaic camera images  
   - minor bug fix in the qfits (preprocess) module.

   05.07.2005:
   Increase of the minor version number:   
   major bug fix in mefsplit:
   The case where input images where of type unsigned INT
   was not treated correctly.

   02.08.2005:
   Increase of the minor version number: 
   - Inclusion of ASTROMETRIX and PHOTOMETRIX scripts
     in the pipeline distribution. They are part of WIFIX
     distribution 1.2 and have minor patches to give
     the products GaBoDS version numbers.
   - new script create_photometrix
   - new script create_astromcats_weights_para.sh
   - new script create_stats_table.sh
   - changes in prepare_coadd_swarp.sh to use external
     headers for the images and to coadd only data
     belonging to a certain chip
   - minor changes to create_skysub_para.sh to change
     the format of two arguments

   25.10.2005:
   Increase of the minor version number:
   - new script make_checkplot_stats.sh	
   - Bug fixes in: photom, mefsplit, imstats, create_norm_para.sh,
     create_weights_para.sh
   - inclusion of the UNIX programs sort, find in progs.ini
     as special parameters (not present in all implementations) 
     are needed.
   - explicit declaration of statistics regions in eclipse
     processing scripts.
   - I improved make_join to get rid of the restriction that
     that the reference column   has to contain ascending 
     numbers starting with 1.	
   - The program album now can use an old header for the output
     image.
   - Extension of mefsplit to resort chip numbers during the
     splitting process.

   05.12.2005:
   Increase of the minor version number:
   - mefsplit: new command line option OUTPUT_DIR
               bug fix for wrong output filenames if
               no header update is requested
   - swarp: changes that allow compilation on Mac/Darwin system	
   - introduction of CFH12K specific scripts and config
     files
   - I started to include NOTUSE and NOTPROCESS flags in
     the scripts. NOTUSE means that chips are not used
     in global statistics or for photometric calibrations
     NOPROCESS means that a chip is not processed at all.
     The update is complete in all scripts on the run
     level.
   - imcombflat has the new caddition mode CLIPMEAN that
     stacks images with an iterative clipped mean. 
     RUN based scripts make use of this feature (they used
     simple median coaddition before)
   - changes of syntaxes for scripts create_runid_para.sh and
     process_science_illum_eclipse_para.sh. The latter
     has additional functionality in that it allows superflatting
     also with the original superflat now. 

   30.01.2006:
   Increase of semi-major version number:
   - Refinement of CFH12K instrument setup periods
   - imcombflat is used again with median coaddition (bias,
     flat, science). 
   - addition of features/change of functionality/change of usage:	
     create_modesub_para.sh, create_astrometrix_astrom_run.sh,
     create_stats_table.sh, make_checkplot_stats.sh,
     prepare_coadd_swarp.sh, mag_distribution_korr.sh
     create_abs_photo_info.sh
   - bug fixes in: transform_ds9_reg.sh, mefsplit, create_stats_table.sh,
     make_join
   - new scripts: create_absphotom_photometrix.sh,
     prepare_coadd_swarp_CFH12K.sh, update_coadd_header.sh
     create_absphot_header.sh
   - new config files: asctoldac_absphotom.conf	

   30.01.2006:
   Increase of minor version number:
   change of behaviour: process_split_WFI_eclipse.sh

   13.02.2006:
   Increase of minor version number:
   bug fixes: update_coadd_header.sh, process_science_eclipse_para.sh

   17.05.2006:
   Increase of minor version number:
   - important internal code structure changes:
     All NUmerical Recipes sorting algorithms were replaced by 
     freely distributable  ones.
   - addition of features/change of usage:
     - MAC OS Darwin systems were included as 'known' systems
     - Changement of the Astrometrix installation scheme.
       Pipeline files no longer override original ASTROMETRIX
       files but modified scripts are directly called. For this,
       new environment variables for changing the PERL environment
       haver to be set during user pipeline setup.
     - the imstats program now reports also lower and upper
       quartiles.
     - make_checkplot_stats.sh, mefsplit
   - bug fixes:
     update_coadd_header.sh, imstats, create_runid_para.sh,
     caldate

   14.06.2006:
   Increase of minor version number:
   - Minor Internal code structure changes:
     nightid, imstats, resample_coadd_swarp_para.sh, 
     perform_coadd_swarp.sh
   - MAJOR bug fixes:
     create_skysub_para.sh

   30.08.2006:
   Increase of minor version number:

   I started to implement a new scheme to treat bad CCDs
   within a mosaic. This was necessary as often only individual
   CCDs are unusable within a mosaic (e.g. readout problems for
   a special CCD). It seems that for the new 1 square degree
   cameras this happens quite often and hence rejecting all the 
   data if only a single CCD is bad is not a serious option.
   The idea is to mask those CCDs with a headerkeyword 'BADCCD'.
   If this keyword is '1' we do not consider that CCD at appropriate
   places. The scheme has been implemented on the SET but not yet
   on the RUN processing level. Please note that this makes the
   former 'NOTUSE' option with which we marked CCDs, that always were
   faulty (e.g. early CFH12K data), superfluous.

   - Internal code structure changes:
     - SExtractor: I included changes proposed on the TERAPIX
       forums to make the code work with gcc compilers having a 
       version number 4.1 or larger.	
   - addition of features/change of usage:
     - swarp: The program accepts a new command line option
       'INPUTIMAGE_LIST'. It expects an ASCII file listing images
       to be processed by swarp. I included this possibility to
       circumvent 'Argument list too long' shell issues when
       many images were given on the command line.
     - I made several scripts more robust against 'ls' commands
       with many images which can result in 'Argument list too long'
       errors. Files are preferably listed via 'find' now
       (create_astrometrix_atrom_run.sh, create_photometrix.sh).
     - create_run_list.sh: The header keyword identifying a RUN is
       now a command line argument.
     - perform_coadd_swarp.sh: The swarp COMBINE type is now an
       (optional) command line argument to the script. If not
       given a WEGHTED mean co-addition if performed.
     - create_stats_table.sh: if an image has a header keyword
       'BADCCD' and if it is set to '1', that chip is not used
       in the calculation of statistics.
     - prepare_coadd_swarp.sh:  if an image has a header keyword
       'BADCCD' and if it is set to '1', it is not considered in
       the co-addition process. 
     - mefsplit: I included the possibility to give header information
       for only a single CCD if only a special chip should be extracted
       from an MEF file.
   - bug fixes:
     bash_functions_include, preprocess
   - New scripts/config files:
     - I included preliminary config files and processing scripts
       for the MEGACAM@MMT instrument.
     - I included scripts to process MEGAPRIME@CFHT Elixir 
       preprocessed data. Special scripts for working with CFHTLS
       data are also included.

   23.09.2006: 
   Increase of semi major version number:

   - Internal code structure changes:
     prepare_coadd_swarp.sh, resample_coadd_swarp_para.sh,
     create_run_list.sh, swarp

   - addition of features/change of usage:
     create_weights_flags_para.sh, create_weights_para.sh.raw:
     possibility to use other retina masks than the default one
     (necessary for CFHT?MEGAPRIME data for instance)
   
   - bug fixes: preanisotropy, perform_coadd_swarp.sh

   - new files: megacam.ret, process_cut_eclipse_para.sh

   06.11.2006: 
   Increase of semi major version number to 0.12.0:
   
   - internal code structure changes:
     bash_functions_include

   - addition of features/change of usage:
     - create_astromcats_weights_para.sh:
       chips marked with BADCCD=1 are not processed

     - decimaltohms, decimaltodms, dmstodecimal,
       hmstodecimal:
       The programs accept files (including stdin) with lists 
       of coordinates to convert

     - do_astrometrix_cat.sh:
       The script accepts also a weight map and detection
       threshold and minarea need to be given on the command 
       line

     - update_coadd_header.sh:
       The comment for the magnitude zeropoint is now
       Vega or AB (configurable via the command line)
   
     - astrom_theli, photom_theli:
       ASTROMETRIX and PHOTOMETRIX take into account chips
       with BADCCD=1; they are not considered in the
       calibrations.
*/

#ifndef __PIPEVERS__
#define __PIPEVERS__ "THELI Pipeline Version: 1.9.5"
#endif
