bin_PROGRAMS = spass
spass_SOURCES = audio_random.h \
		audio_random.cpp \
		audio_random_alsa.h \
		audio_random_alsa.cpp \
		audio_random_oss.h \
		audio_random_oss.cpp \
		audio_random_portaudio.h \
		audio_random_portaudio.cpp \
		config.h \
		main.cpp \
		diceware8k.c \
		md5.c \
		md5.h

#AM_CPPFLAGS = "$(all_includes)"
LDADD = -lboost_program_options -lportaudio
