CC = i386-gwos-gcc
CFLAGS = -c -O0
INCLUDE_PATH = -I./include/
PROG = DOOM

SRCS += doom_gwos.o
SRCS += src/am_map.o
SRCS += src/d_items.o
SRCS += src/d_main.o
SRCS += src/d_net.o
SRCS += src/DOOM.o
SRCS += src/doomdef.o
SRCS += src/doomstat.o
SRCS += src/dstrings.o
SRCS += src/f_finale.o
SRCS += src/f_wipe.o
SRCS += src/g_game.o
SRCS += src/hu_lib.o
SRCS += src/hu_stuff.o
SRCS += src/i_net.o
SRCS += src/info.o
SRCS += src/i_sound.o
SRCS += src/i_system.o
SRCS += src/i_video.o
SRCS += src/m_argv.o
SRCS += src/m_bbox.o
SRCS += src/m_cheat.o
SRCS += src/m_fixed.o
SRCS += src/m_menu.o
SRCS += src/m_misc.o
SRCS += src/m_random.o
SRCS += src/m_swap.o
SRCS += src/p_ceilng.o
SRCS += src/p_doors.o
SRCS += src/p_enemy.o
SRCS += src/p_floor.o
SRCS += src/p_inter.o
SRCS += src/p_lights.o
SRCS += src/p_map.o
SRCS += src/p_maputl.o
SRCS += src/p_mobj.o
SRCS += src/p_plats.o
SRCS += src/p_pspr.o
SRCS += src/p_saveg.o
SRCS += src/p_setup.o
SRCS += src/p_sight.o
SRCS += src/p_spec.o
SRCS += src/p_switch.o
SRCS += src/p_telept.o
SRCS += src/p_tick.o
SRCS += src/p_user.o
SRCS += src/r_bsp.o
SRCS += src/r_data.o
SRCS += src/r_draw.o
SRCS += src/r_main.o
SRCS += src/r_plane.o
SRCS += src/r_segs.o
SRCS += src/r_sky.o
SRCS += src/r_things.o
SRCS += src/sounds.o
SRCS += src/s_sound.o
SRCS += src/st_lib.o
SRCS += src/st_stuff.o
SRCS += src/tables.o
SRCS += src/v_video.o
SRCS += src/wi_stuff.o
SRCS += src/w_wad.o
SRCS += src/z_zone.o

%.o : %.c
	$(CC) -c $(CFLAGS) $(INCLUDE_PATH) $< -o $@
%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

build: $(SRCS)
	$(CC) $(SRCS) -o $(PROG).elf

clean:
	rm $(SRCS)