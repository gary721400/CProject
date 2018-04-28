//! @file explorer.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the explorer interface.
//! Explorer file provide a routines pluged on navigation routines which permit to explore a disk, directory or playlist with a filter extension
//! Also, this one include a feature repeat and random
//!
//! @version 1.5 snd3-dvk-1_9_5 $Id: explorer.c,v 1.5 2007/10/29 15:36:39 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _explorer_c_
#define _explorer_c_

//_____  I N C L U D E S ___________________________________________________
#include "conf\conf_explorer.h"
#include "explorer.h"
#include "file.h"
//#include "modules\control_access\ctrl_access.h"
//#include LIB_MEM
#include "stdio.h"
#include "sio\com.h"
#include "mmi\shared\com_var.h"
#include "mmi\player\mmi_player.h"
//#include "MP3_appl.h"    // MP3 ApplicationLayer header file

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"


#pragma DATA = EXPLORER_DATA
//#pragma CODE = EXPLORER_CODE
#pragma CODE = MMI_PLAYER_CODE

ImageDeclareType( EXPLORER_TEMP_CODE02 );


//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N S ______________________________________________

//_MEM_TYPE_SLOW_   U16         g_exp_u16_nb;                          //!< Number of file in playlist OR number of filter file in directory 
//_MEM_TYPE_SLOW_   U16         g_exp_u16_dir_level;                   //!< Current directory level selected (only use for disk and sub directory explore)
//_MEM_TYPE_SLOW_   FS_STRING   g_exp_filter;                          //!< Filter use to explore directory only (no use in playlist)
//extern   xdata    AL_Variables   varAl;

//_____ D E C L A R A T I O N S ____________________________________________

void           exp_rand_init     ( void );
void           exp_countnbfile   ( void );
Exp_mov_state  exp_mov_explorer  ( Bool b_direction );
Exp_mov_state  exp_mov_playlist  ( Bool b_direction );

#if 0
//! This fonction return the mode used
//!
//! @return mode used
//!
Exp_mode exp_getmode( void )
{
   return g_exp_exp_mode;
}


//! This fonction select the loop mode (EXP_MODE_DISKS, EXP_MODE_DIRONLY, EXP_MODE_DIRSUB, EXP_MODE_PLAYLIST)
//! It reinit the folder level at the current position
//!
//! @param exp_mode    mode to select
//!
Bool  exp_setmode( Exp_mode exp_mode )
{
   if( (g_exp_exp_mode    != exp_mode)
   &&  (EXP_MODE_PLAYLIST == g_exp_exp_mode) )
      return FALSE;
   g_exp_u16_dir_level = 0;
   g_exp_exp_mode = exp_mode;
   exp_rand_init();
   return TRUE;
}


//! This fonction return the random used
//!
//! @return random used
//!
Exp_rand exp_getrand( void )
{
   return g_exp_rand;
}


//! This fonction select the random used
//!
//! @param rand   random to use
//!
void  exp_setrand( Exp_rand rand )
{
   g_exp_rand = rand;
   exp_rand_init();
   // If current file in rand table
   if( g_exp_u16_pos <= (EXP_MAX_RANGE_RAND*8) )
   {
      // Set current position in rand table
      U8 u8_pos_rand = g_exp_u16_pos-1;
      g_exp_rand_tab[u8_pos_rand/8] |= 1<<(u8_pos_rand%8);
   }
}


//! This fonction initialize the random table
//!
void  exp_rand_init( void )
{
   // Init rand table
   memset( g_exp_rand_tab , 0 , EXP_MAX_RANGE_RAND );
   g_exp_u16_rand_pos = 0;
}


//! This fonction return a random position no used
//!
U16  exp_rand_choose( void )
{
   U8 u8_i;
   U8 u8_nb_file_rand;
   U8 u8_pos_file_rand;
   
exp_rand_choose_restart:
   if( g_exp_u16_nb == g_exp_u16_rand_pos )
      return 0;   // End of rand in dir, then no file possible

   // Compute the number of rand file in rand table
   if( (g_exp_u16_nb-g_exp_u16_rand_pos) < (EXP_MAX_RANGE_RAND*8) )
      u8_nb_file_rand = g_exp_u16_nb-g_exp_u16_rand_pos;
   else
      u8_nb_file_rand = EXP_MAX_RANGE_RAND*8;

   // Table full ?
   u8_i = 0;
   while( 1 )
   {
      if( u8_nb_file_rand == u8_i )
      {
         // Table full then init a next rand table
         g_exp_u16_rand_pos += u8_nb_file_rand;
         memset( g_exp_rand_tab , 0 , EXP_MAX_RANGE_RAND );
         goto exp_rand_choose_restart;
      }
      if( 0 == (g_exp_rand_tab[u8_i/8] & (1<<(u8_i%8))) )
         break;   // A no used position find
      u8_i++;
   }

   // Get rand value
   EXP_GET_RAND(u8_i);

   // Compute the current position in rand table
   u8_pos_file_rand = g_exp_u16_pos-g_exp_u16_rand_pos-1;

   // Scan no used position
   while( 0 != u8_i )
   {
      u8_pos_file_rand++;
      if( u8_pos_file_rand >= u8_nb_file_rand )
         u8_pos_file_rand = 0;
      if( 0 == (g_exp_rand_tab[u8_pos_file_rand/8] & (1<<(u8_pos_file_rand%8))) )
         u8_i--;
   }
   // position no used find
   g_exp_rand_tab[u8_pos_file_rand/8] |= 1<<(u8_pos_file_rand%8); // Update table
   return (g_exp_u16_rand_pos+u8_pos_file_rand+1);                // Send position
}


//! This fonction init the filter extension used in the loop
//!
//! @param sz_filterext   filter
//!
void	exp_init( const FS_STRING sz_filterext )
{
   g_exp_filter = sz_filterext;  // Ignored in play list mode
}
#endif


#pragma CODE = EXPLORER_TEMP_CODE00

//! This fonction open the loop at current position
//!
//! @param mode   explorer mode
//! @param pos    if( loop mode = EXP_MODE_PLAYLIST ) position in play list to go
//!               else folder level to init
//!
Bool  exp_open( Exp_mode mode , U16 pos )
{
   Exp_mov_state state;

   // Init loop control values
   //g_exp_u16_dir_level  =  pos;
   state  = mode;
   pos = pos;

   {
#if 0
      if( nav_file_checkext( player_dir ))
      {
         // Compute the current position in filter list
         /*u16_current_pos = 1;
         while( nav_filelist_set( 0 , FS_FIND_PREV ) )
         {
            if( !nav_file_isdir())
               if( nav_file_checkext( g_exp_filter ))
                  u16_current_pos++;
         }*/   
         // Compute the number of valid file in currrent dir
         exp_countnbfile();
         // Restore the position in filter list
         /*g_exp_u16_pos = u16_current_pos;
         nav_filelist_reset();
         while( nav_filelist_set( 0 , FS_FIND_NEXT ) )
         {
            if( (!nav_file_isdir())
            &&  (nav_file_checkext( g_exp_filter )) )
            {
               u16_current_pos--;
               if( 0 == u16_current_pos )
                  break;   // Valid file selected
            }
         }*/    
      }
      else
#endif
      {
         //exp_countnbfile();
         ( void )nav_filelist_reset();
         SDKLoadCode( EXPLORER_TEMP_CODE02 );
         state = exp_mov_explorer(FS_FIND_NEXT);
         if((EXP_MOV_OK_LOOP != state)
         && (EXP_MOV_OK      != state) )
            return FALSE;
      }
   }
   return TRUE;
}


//! This fonction close the loop and send the actual position
//!
#if 0
U16  exp_close( void )
{
   if( EXP_MODE_PLAYLIST == g_exp_exp_mode )
   {
      pl_main_close();
      return g_exp_u16_pos;
   }
   return g_exp_u16_dir_level;
}


//! This fonction return the number of file in list or valid file in current directory
//!
U16   exp_getnb( void )
{
   return g_exp_u16_nb;
}


//! This fonction return the position
//!
U16   exp_getpos( void )
{
   return g_exp_u16_pos;
}
#endif


#pragma CODE = EXPLORER_TEMP_CODE01

//! This fonction select a file in loop
//!
//! @param b_direction  direction of navigation (FS_FIND_NEXT or FS_FIND_PREV)
//!
//! @return    the state of the action
//!
Exp_mov_state   exp_mov( Bool b_direction )
{
#if 0
   if( EXP_MODE_PLAYLIST == g_exp_exp_mode )
      return exp_mov_playlist( b_direction );
   else
#endif
   {
      SDKLoadCode( EXPLORER_TEMP_CODE02 );
      return exp_mov_explorer( b_direction );
   }
}


#pragma CODE = EXPLORER_TEMP_CODE02

//! This fonction select a file in EXPLORER loop
//!
//! @param b_direction  direction of navigation (FS_FIND_NEXT or FS_FIND_PREV)
//!
//! @return    the state of the action
//!
Exp_mov_state   exp_mov_explorer( Bool b_direction )
{
   //_MEM_TYPE_SLOW_ U8  ext_filemusic[]= "wav";
   Bool b_endof_loop;
   //Bool b_new_dir;
   //U8   u8_drive;
   //U16  u16_mov_pos;

   /*if( EXP_RAND_ON == g_exp_rand )
   {
      u16_mov_pos = exp_rand_choose();
      if( 0 != u16_mov_pos )
         b_direction = ( u16_mov_pos > g_exp_u16_pos )? FS_FIND_NEXT : FS_FIND_PREV;
   }*/

//exp_mov_beginning:
   b_endof_loop = FALSE;
   //b_new_dir    = FALSE;

   // Here, explorer folder
   while(1) // Main loop
   {
      while(1)
      {
         if( nav_filelist_set( 0 , b_direction ))
            break;   // exist while(1) to check file or directory selected
#if 0
         if( FS_ERR_NO_FIND != fs_g_status )
         {
            /*if( EXP_MODE_DISKS == g_exp_exp_mode )
               goto exp_mov_changedisk;
            else*/
               goto exp_mov_diskerror;
         }
#endif
         // End of loop
         if( b_endof_loop )
            return EXP_MOV_EMPTY;   // Loop empty
         b_endof_loop = TRUE;
         ( void )nav_filelist_reset();      // Reset directory
         if( FS_FIND_PREV == b_direction )
         {
            while( nav_filelist_set( 0, FS_FIND_NEXT ));
            break;
         }
#if 0
         // No other dir or file in current dir then go to parent dir
         /*if((0 == g_exp_u16_dir_level)
         && ((EXP_MODE_DISK != g_exp_exp_mode) && (EXP_MODE_DISKS != g_exp_exp_mode)) )
         {
            // End of loop
            if( b_endof_loop )
               return EXP_MOV_EMPTY;  // Loop empty
            b_endof_loop = TRUE;
            nav_filelist_reset();      // Reset directory
            goto exp_mov_newdir;
         }*/

         // Go to parent dir
         g_exp_u16_dir_level--;
         if( nav_dir_gotoparent() )
         {  // Remark, nav_dir_gotoparent() routine go to in parent dir and select the children dir in list
            b_new_dir = TRUE;
            continue;         // Scan parent dir
         }

         if((FS_ERR_IS_ROOT  != fs_g_status)
         /*|| ((EXP_MODE_DISKS != g_exp_exp_mode) && (EXP_MODE_DISK != g_exp_exp_mode)) */)
            goto exp_mov_diskerror;
         //if( EXP_MODE_DISK == g_exp_exp_mode )
         {
            // End of loop
            if( b_endof_loop )
               return EXP_MOV_EMPTY;  // Loop empty
            b_endof_loop = TRUE;
            nav_filelist_reset();      // Reset directory
            goto exp_mov_newdir;
         }   
         // Change disk
//exp_mov_changedisk:
         while(1)
         {
            // go to next drive
            u8_drive = nav_drive_get();
            if( FS_FIND_PREV == b_direction )
            {
               if( 0 == u8_drive )
               {
                  u8_drive = nav_drive_nb()-1;
                  if( b_endof_loop )
                     return EXP_MOV_EMPTY;  // Loop empty
                  b_endof_loop = TRUE;
               }else{
                  u8_drive--;
               }
            }else{
               u8_drive++;
               if( u8_drive >= nav_drive_nb() )
               {
                  u8_drive=0;
                  if( b_endof_loop )
                     return EXP_MOV_EMPTY;  // Loop empty
                  b_endof_loop = TRUE;
               }
            }
            if( !nav_drive_set(u8_drive) )
               continue;   // Go to other disk
            if( !nav_partition_mount() )
               continue;   // Go to other disk
            goto exp_mov_newdir;  // Disk OK
         }
#endif
      } // end of while (1)

//exp_mov_checkfile:
      // Check file or directory selected
      if( nav_file_isdir())
      {
         // here, a new directory is found and is selected
         //if( EXP_MODE_DIRONLY != g_exp_exp_mode )
         {
#if 1
            continue;
#else
            // Enter in dir
            if( !nav_dir_cd())
            {
               /*if( EXP_MODE_DISKS == g_exp_exp_mode)
                  goto exp_mov_changedisk;
               else*/
                  goto exp_mov_diskerror;
            }
            g_exp_u16_dir_level++;


exp_mov_newdir:
            b_new_dir = TRUE;
            if( FS_FIND_PREV == b_direction )
            {
               if( nav_filelist_eol() )
                  continue;  // Directory empty then go to restart first loop to jump in parent dir
               // Go to end of directory
               while( nav_filelist_set( 0 , FS_FIND_NEXT ) );
               // check file or directory selected
               goto exp_mov_checkfile;
            }
#endif
         }
      }
      else
      {
         // here, a new file is found and is selected
         if( nav_file_checkext(( U8 *)MP3_EXT ))
         {
            if( FS_FIND_PREV == b_direction )
            {
               if( !nav_filelist_set( 0, FS_FIND_PREV ))
                  ( void )nav_filelist_reset();      // Reset directory
               ( void )nav_filelist_set( 0, FS_FIND_NEXT );
            }
            audio_is_wav = 0;
            //wav_encrypt = FALSE;
            //SendString((U8 *)"mp3\r\n");
#if 0
            // The file have the good extension
            if( b_new_dir )
            {
               // It is the first file of a new dir
               b_new_dir = FALSE;
               // Compute nb valid file in this dir
               u16_mov_pos = nav_filelist_get();   // Save pos in list no filtered
               exp_countnbfile();
               nav_filelist_goto(u16_mov_pos);     // Restore pos in list no filtered

               // Init navigation position
               /*if( FS_FIND_NEXT == b_direction )
                  g_exp_u16_pos = 1;
               else
                  g_exp_u16_pos = g_exp_u16_nb;*/

            }
            /*else
            {
               ( FS_FIND_NEXT == b_direction )? g_exp_u16_pos++ : g_exp_u16_pos--;

            }*/
#endif
            // Here, the file is found
            return (( b_endof_loop )? EXP_MOV_OK_LOOP : EXP_MOV_OK);
         }
         if( nav_file_checkext(( U8 *)RECORD_EXT ))
         {
            if( FS_FIND_PREV == b_direction )
            {
               if( !nav_filelist_set( 0, FS_FIND_PREV ))
                  ( void )nav_filelist_reset();      // Reset directory
               ( void )nav_filelist_set( 0, FS_FIND_NEXT );
            }
            audio_is_wav = 1;
            //wav_encrypt = FALSE;
            // Here, the file is found
            //SendString((U8 *)"wav\r\n");
            return (( b_endof_loop )? EXP_MOV_OK_LOOP : EXP_MOV_OK);
         }
      }
   } // end of first while(1)
#if 0
exp_mov_diskerror:
   if( FS_ERR_HW_NO_PRESENT == fs_g_status)
   {
      //if( EXP_MODE_DISKS != g_exp_exp_mode )
      return EXP_MOV_DISKNOPRESENT;
      //goto exp_mov_changedisk;
   }
#endif
   return EXP_MOV_DISKERROR;
}

#if 0
//! This fonction compute the number of valid file in current directory
//!
void exp_countnbfile( void )
{
   U16 g_exp_u16_nb = 0;
   nav_filelist_reset();
   while( nav_filelist_set( 0 , FS_FIND_NEXT ) )
   {
      if( !nav_file_isdir())
      {
         if( nav_file_checkext( player_dir ))
            g_exp_u16_nb++;
      }
   }
}

//! This fonction select a file in PLAYLIST
//!
//! @param b_direction  direction of navigation (FS_FIND_NEXT or FS_FIND_PREV)
//!
//! @return    the state of the action
//!
Exp_mov_state   exp_mov_playlist( Bool b_direction )
{
   Bool b_endof_loop;
   b_endof_loop = FALSE;

   if( EXP_RAND_ON == g_exp_rand )
   {
      g_exp_u16_pos = exp_rand_choose();
      if( 0 == g_exp_u16_pos )
      {
         // Loop finish, then reinit rand and chosse other value
         b_endof_loop = TRUE;
         exp_rand_init();
         g_exp_u16_pos = exp_rand_choose();
      }
      pl_nav_setpos( g_exp_u16_pos );
   }
   else
   {
      switch( pl_nav( b_direction ))
      {
         case PL_NAV_OK:
         case PL_NAV_DEAD:
         break;

         case PL_NAV_FAIL:
         // Loop finish or error
         b_endof_loop = TRUE; 
         if( FS_ERR_PL_LST_END == fs_g_status )
            pl_nav_setpos( 1 );              // end of list then go to beginning
         else if ( FS_ERR_PL_LST_BEG == fs_g_status )
            pl_nav_setpos( g_exp_u16_nb );   // beginning of list then go to end
         else
         	return EXP_MOV_DISKERROR;
         break;
      }
   }
   g_exp_u16_pos = pl_nav_getpos();
   if( b_endof_loop )
      return EXP_MOV_OK_LOOP;
   return EXP_MOV_OK;
}
#endif

#endif  //_explorer_c_
