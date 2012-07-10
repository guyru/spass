bin_PROGRAMS = spass
spass_SOURCES = audio_random.h \
		audio_random.cpp \
		config.h \
		main.cpp \
		diceware8k.c \
		md5.c \
		md5.h

LDADD = -lboost_program_options

if WITH_ALSA
spass_SOURCES += audio_random_alsa.h \
	audio_random_alsa.cpp
CFLAGS += $(ALSA_CFLAGS)
LDADD += $(ALSA_LIBS)
endif

if WITH_PORTAUDIO
spass_SOURCES += audio_random_portaudio.h \
	audio_random_portaudio.cpp
CFLAGS += $(PORTAUDIO_CFLAGS)
LDADD += $(PORTAUDIO_LIBS)
endif

if WITH_OSS
spass_SOURCES += audio_random_oss.h \
	audio_random_oss.cpp
endif

#AM_CPPFLAGS = "$(all_includes)"
