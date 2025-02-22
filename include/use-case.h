/**
 * \file include/use-case.h
 * \brief use case interface for the ALSA driver
 * \author Liam Girdwood <lrg@slimlogic.co.uk>
 * \author Stefan Schmidt <stefan@slimlogic.co.uk>
 * \author Jaroslav Kysela <perex@perex.cz>
 * \author Justin Xu <justinx@slimlogic.co.uk>
 * \date 2008-2010
 */
/*
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation; either version 2.1 of
 *  the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *  Copyright (C) 2008-2010 SlimLogic Ltd
 *  Copyright (C) 2010 Wolfson Microelectronics PLC
 *  Copyright (C) 2010 Texas Instruments Inc.
 *
 *  Support for the verb/device/modifier core logic and API,
 *  command line tool and file parser was kindly sponsored by
 *  Texas Instruments Inc.
 *  Support for multiple active modifiers and devices,
 *  transition sequences, multiple client access and user defined use
 *  cases was kindly sponsored by Wolfson Microelectronics PLC.
 */

#ifndef __ALSA_USE_CASE_H
#define __ALSA_USE_CASE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <alsa/asoundlib.h>

/**
 *  \defgroup ucm Use Case Interface
 *  The ALSA Use Case manager interface.
 *  See \ref Usecase page for more details.
 *  \{
 */

/*! \page Usecase ALSA Use Case Interface
 *
 * The use case manager works by configuring the sound card ALSA kcontrols to
 * change the hardware digital and analog audio routing to match the requested
 * device use case. The use case manager kcontrol configurations are stored in
 * easy to modify text files.
 *
 * An audio use case can be defined by a verb and device parameter. The verb
 * describes the use case action i.e. a phone call, listening to music, recording
 * a conversation etc. The device describes the physical audio capture and playback
 * hardware i.e. headphones, phone handset, bluetooth headset, etc.
 *
 * It's intended clients will mostly only need to set the use case verb and
 * device for each system use case change (as the verb and device parameters
 * cover most audio use cases).
 *
 * However there are times when a use case has to be modified at runtime. e.g.
 *
 *  + Incoming phone call when the device is playing music
 *  + Recording sections of a phone call
 *  + Playing tones during a call.
 *
 * In order to allow asynchronous runtime use case adaptations, we have a third
 * optional modifier parameter that can be used to further configure
 * the use case during live audio runtime.
 *
 * This interface allows clients to :-
 *
 *  + Query the supported use case verbs, devices and modifiers for the machine.
 *  + Set and Get use case verbs, devices and modifiers for the machine.
 *  + Get the ALSA PCM playback and capture device PCMs for use case verb,
 *     use case device and modifier.
 *  + Get the TQ parameter for each use case verb, use case device and
 *     modifier.
 *  + Get the ALSA master playback and capture volume/switch kcontrols
 *     or mixer elements for each use case.
 */


/*
 * Use Case Verb.
 *
 * The use case verb is the main device audio action. e.g. the "HiFi" use
 * case verb will configure the audio hardware for HiFi Music playback
 * and capture.
 */
#define SND_USE_CASE_VERB_INACTIVE		"Inactive"		/**< Inactive Verb */
#define SND_USE_CASE_VERB_HIFI			"HiFi"			/**< HiFi Verb */
#define SND_USE_CASE_VERB_HIFI_LOW_POWER	"HiFi Low Power"	/**< HiFi Low Power Verb */
#define SND_USE_CASE_VERB_VOICE			"Voice"			/**< Voice Verb */
#define SND_USE_CASE_VERB_VOICE_LOW_POWER	"Voice Low Power"	/**< Voice Low Power Verb */
#define SND_USE_CASE_VERB_VOICECALL		"Voice Call"		/**< Voice Call Verb */
#define SND_USE_CASE_VERB_IP_VOICECALL		"Voice Call IP"		/**< Voice Call IP Verb */
#define SND_USE_CASE_VERB_ANALOG_RADIO		"FM Analog Radio"	/**< FM Analog Radio Verb */
#define SND_USE_CASE_VERB_DIGITAL_RADIO		"FM Digital Radio"	/**< FM Digital Radio Verb */
/* add new verbs to end of list */


/*
 * Use Case Device.
 *
 * Physical system devices the render and capture audio. Devices can be OR'ed
 * together to support audio on simultaneous devices.
 *
 * If multiple devices with the same name exists, the number suffixes should
 * be added to these names like HDMI1,HDMI2,HDMI3 etc. No number gaps are
 * allowed. The names with numbers must be continuous. It is allowed to put
 * a whitespace between name and index (like 'Line 1') for the better
 * readability. The device names 'Line 1' and 'Line1' are equal for
 * this purpose.
 *
 * If EnableSequence/DisableSequence controls independent paths in the hardware
 * it is also recommended to split playback and capture UCM devices and use
 * the number suffixes. Example use case: Use the integrated microphone
 * in the laptop instead the microphone in headphones.
 *
 * The preference of the devices is determined by the priority value.
 */
#define SND_USE_CASE_DEV_NONE		"None"		/**< None Device */
#define SND_USE_CASE_DEV_SPEAKER	"Speaker"	/**< Speaker Device */
#define SND_USE_CASE_DEV_LINE		"Line"		/**< Line Device */
#define SND_USE_CASE_DEV_MIC            "Mic"           /**< Microphone Device */
#define SND_USE_CASE_DEV_HEADPHONES	"Headphones"	/**< Headphones Device */
#define SND_USE_CASE_DEV_HEADSET	"Headset"	/**< Headset Device */
#define SND_USE_CASE_DEV_HANDSET	"Handset"	/**< Handset Device */
#define SND_USE_CASE_DEV_BLUETOOTH	"Bluetooth"	/**< Bluetooth Device */
#define SND_USE_CASE_DEV_EARPIECE	"Earpiece"	/**< Earpiece Device */
#define SND_USE_CASE_DEV_SPDIF		"SPDIF"		/**< SPDIF Device */
#define SND_USE_CASE_DEV_HDMI		"HDMI"		/**< HDMI Device */
#define SND_USE_CASE_DEV_USB		"USB"		/**< USB Device (multifunctional) */
#define SND_USE_CASE_DEV_DIRECT		"Direct"	/**< Direct Device (no channel remapping), (e.g. ProAudio usage) */
/* add new devices to end of list */


/*
 * Use Case Modifiers.
 *
 * The use case modifier allows runtime configuration changes to deal with
 * asynchronous events.
 *
 * If multiple modifiers with the same name exists, the number suffixes should
 * be added to these names like 'Echo Reference 1','Echo Reference 2' etc.
 * No number gaps are allowed. The names with numbers must be continuous.
 * It is allowed to put a whitespace between name and index for the better
 * readability. The modifier names 'Something 1' and 'Something1' are equal
 * for this purpose.
 *
 * e.g. to record a voice call :-
 *  1. Set verb to SND_USE_CASE_VERB_VOICECALL (for voice call)
 *  2. Set modifier SND_USE_CASE_MOD_CAPTURE_VOICE when capture required.
 *  3. Call snd_use_case_get("CapturePCM") to get ALSA source PCM name
 *     with captured voice pcm data.
 *
 * e.g. to play a ring tone when listenin to MP3 Music :-
 *  1. Set verb to SND_USE_CASE_VERB_HIFI (for MP3 playback)
 *  2. Set modifier to SND_USE_CASE_MOD_PLAY_TONE when incoming call happens.
 *  3. Call snd_use_case_get("PlaybackPCM") to get ALSA PCM sink name for
 *     ringtone pcm data.
 */
#define SND_USE_CASE_MOD_CAPTURE_VOICE		"Capture Voice"		/**< Capture Voice Modifier */
#define SND_USE_CASE_MOD_CAPTURE_MUSIC		"Capture Music"		/**< Capture Music Modifier */
#define SND_USE_CASE_MOD_PLAY_MUSIC		"Play Music"		/**< Play Music Modifier */
#define SND_USE_CASE_MOD_PLAY_VOICE		"Play Voice"		/**< Play Voice Modifier */
#define SND_USE_CASE_MOD_PLAY_TONE		"Play Tone"		/**< Play Tone Modifier */
#define SND_USE_CASE_MOD_ECHO_REF		"Echo Reference"	/**< Echo Reference Modifier */
/* add new modifiers to end of list */


/**
 * TQ - Tone Quality
 *
 * The interface allows clients to determine the audio TQ required for each
 * use case verb and modifier. It's intended as an optional hint to the
 * audio driver in order to lower power consumption.
 *
 */
#define SND_USE_CASE_TQ_MUSIC		"Music"		/**< Music Tone Quality */
#define SND_USE_CASE_TQ_VOICE		"Voice"		/**< Voice Tone Quality */
#define SND_USE_CASE_TQ_TONES		"Tones"		/**< Tones Tone Quality */

/** use case container */
typedef struct snd_use_case_mgr snd_use_case_mgr_t;

/**
 * \brief Create an identifier
 * \param fmt Format (sprintf like)
 * \param ... Optional arguments for sprintf like format
 * \return Allocated string identifier or NULL on error
 */
char *snd_use_case_identifier(const char *fmt, ...);

/**
 * \brief Free a string list
 * \param list The string list to free
 * \param items Count of strings
 * \return Zero if success, otherwise a negative error code
 */
int snd_use_case_free_list(const char *list[], int items);

/**
 * \brief Obtain a list of entries
 * \param uc_mgr Use case manager (may be NULL - card list)
 * \param identifier (may be NULL - card list)
 * \param list Returned allocated list
 * \return Number of list entries if success, otherwise a negative error code
 *
 * Defined identifiers:
 *   - NULL			- get card list
 *				 (in pair cardname+comment)
 *   - _verbs			- get verb list
 *				  (in pair verb+comment)
 *   - _devices[/{verb}]	- get list of supported devices
 *				  (in pair device+comment)
 *   - _modifiers[/{verb}]	- get list of supported modifiers
 *				  (in pair modifier+comment)
 *   - TQ[/{verb}]		- get list of TQ identifiers
 *   - _enadevs			- get list of enabled devices
 *   - _enamods			- get list of enabled modifiers
 *
 *   - _identifiers/{modifier}|{device}[/{verb}]     - list of value identifiers
 *   - _supporteddevs/{modifier}|{device}[/{verb}]   - list of supported devices
 *   - _conflictingdevs/{modifier}|{device}[/{verb}] - list of conflicting devices
 *
 *   Note that at most one of the supported/conflicting devs lists has
 *   any entries, and when neither is present, all devices are supported.
 *
 */
int snd_use_case_get_list(snd_use_case_mgr_t *uc_mgr,
                          const char *identifier,
                          const char **list[]);


/**
 * \brief Get current - string
 * \param uc_mgr Use case manager
 * \param identifier 
 * \param value Value pointer
 * \return Zero if success, otherwise a negative error code
 *
 * Note: The returned string is dynamically allocated, use free() to
 * deallocate this string. (Yes, the value parameter shouldn't be marked as
 * "const", but it's too late to fix it, sorry about that.)
 *
 * Known identifiers:
 *   - NULL 		- return current card
 *   - _verb		- return current verb
 *   - _file		- return configuration file loaded for current card
 *   - _alibcfg		- return private alsa-lib's configuration for current card
 *   - _alibpref	- return private alsa-lib's configuration device prefix for current card
 *
 *   - [=]{NAME}[/[{modifier}|{/device}][/{verb}]]
 *                      - value identifier {NAME}
 *                      - Search starts at given modifier or device if any,
 *                          else at a verb
 *                      - Search starts at given verb if any,
 *                          else current verb
 *                      - Searches modifier/device, then verb, then defaults
 *                      - Specify a leading "=" to search only the exact
 *                        device/modifier/verb specified, and not search
 *                        through each object in turn.
 *                      - Examples:
 *                          - "PlaybackPCM/Play Music"
 *                          - "CapturePCM/SPDIF"
 *                          - From ValueDefaults only:
 *                              "=Variable"
 *                          - From current active verb:
 *                              "=Variable//"
 *                          - From verb "Verb":
 *                              "=Variable//Verb"
 *                          - From "Modifier" in current active verb:
 *                              "=Variable/Modifier/"
 *                          - From "Modifier" in "Verb":
 *                              "=Variable/Modifier/Verb"
 *
 * Recommended names for values:
 *   - Linked
 *      - value "True" or "1" (case insensitive)
 *      - this is a linked UCM card
 *      - don't use this UCM card, because the other UCM card refers devices
 *      - valid only in the ValueDefaults section (query '=Linked')
 *   - TQ
 *      - Tone Quality
 *   - Priority
 *      - priority value (1-10000), higher value means higher priority
 *      - valid only for verbs
 *      - for devices - PlaybackPriority and CapturePriority
 *   - PlaybackPCM
 *      - full PCM playback device name
 *   - PlaybackPCMIsDummy
 *      - Valid values: "yes" and "no". If set to "yes", the PCM named by the
 *        PlaybackPCM value is a dummy device, meaning that opening it enables
 *        an audio path in the hardware, but writing to the PCM device has no
 *        effect.
 *   - CapturePCM
 *      - full PCM capture device name
 *   - CapturePCMIsDummy
 *      - Valid values: "yes" and "no". If set to "yes", the PCM named by the
 *        CapturePCM value is a dummy device, meaning that opening it enables
 *        an audio path in the hardware, but reading from the PCM device has no
 *        effect.
 *   - PlaybackRate
 *      - playback device sample rate
 *   - PlaybackChannels
 *      - playback device channel count
 *   - PlaybackCTL
 *      - playback control device name
 *   - PlaybackVolume
 *      - playback control volume identifier string
 *	- can be parsed using snd_use_case_parse_ctl_elem_id()
 *   - PlaybackSwitch
 *      - playback control switch identifier string
 *	- can be parsed using snd_use_case_parse_ctl_elem_id()
 *   - PlaybackPriority
 *      - priority value (1-10000), higher value means higher priority
 *   - CaptureRate
 *      - capture device sample rate
 *   - CaptureChannels
 *      - capture device channel count
 *   - CaptureCTL
 *      - capture control device name
 *   - CaptureVolume
 *      - capture control volume identifier string
 *	- can be parsed using snd_use_case_parse_ctl_elem_id()
 *   - CaptureSwitch
 *      - capture control switch identifier string
 *	- can be parsed using snd_use_case_parse_ctl_elem_id()
 *   - CapturePriority
 *      - priority value (1-10000), higher value means higher priority
 *   - PlaybackMixer
 *      - name of playback mixer
 *   - PlaybackMixerElem
 *      - mixer element playback identifier
 *	- can be parsed using snd_use_case_parse_selem_id()
 *   - PlaybackMasterElem
 *      - mixer element playback identifier for the master control
 *	- can be parsed using snd_use_case_parse_selem_id()
 *   - PlaybackMasterType
 *      - type of the master volume control
 *      - Valid values: "soft" (software attenuation)
 *   - CaptureMixer
 *      - name of capture mixer
 *   - CaptureMixerElem
 *      - mixer element capture identifier
 *	- can be parsed using snd_use_case_parse_selem_id()
 *   - CaptureMasterElem
 *      - mixer element playback identifier for the master control
 *	- can be parsed using snd_use_case_parse_selem_id()
 *   - CaptureMasterType
 *      - type of the master volume control
 *      - Valid values: "soft" (software attenuation)
 *   - EDIDFile
 *      - Path to EDID file for HDMI devices
 *   - JackCTL
 *      - jack control device name
 *   - JackControl
 *      - jack control identificator
 *      - can be parsed using snd_use_case_parse_ctl_elem_id()
 *      - UCM configuration files should contain both JackControl and JackDev
 *        when possible, because applications are likely to support only one
 *        or the other
 *   - JackDev
 *      - the input device id of the jack (if the full input device path is
 *        /dev/input/by-id/foo, the JackDev value should be "foo")
 *      - UCM configuration files should contain both JackControl and JackDev
 *        when possible, because applications are likely to support only one
 *        or the other
 *   - JackHWMute
 *	  If this value is set, it indicates that when the jack is plugged
 *	  in, the hardware automatically mutes some other device(s). The
 *	  value is a space-separated list of device names. If the device
 *	  name contains space, it must be enclosed to ' or ", e.g.:
 *		JackHWMute "'Dock Headphone' Headphone"
 *        Note that JackHWMute should be used only when the hardware enforces
 *        the automatic muting. If the hardware doesn't enforce any muting, it
 *        may still be tempting to set JackHWMute to trick upper software layers
 *        to e.g. automatically mute speakers when headphones are plugged in,
 *        but that's application policy configuration that doesn't belong
 *        to UCM configuration files.
 *   - MinBufferLevel
 *	- This is used on platform where reported buffer level is not accurate.
 *	  E.g. "512", which holds 512 samples in device buffer. Note: this will
 *	  increase latency.
 */
int snd_use_case_get(snd_use_case_mgr_t *uc_mgr,
                     const char *identifier,
                     const char **value);

/**
 * \brief Get current - integer
 * \param uc_mgr Use case manager
 * \param identifier 
 * \param value result 
 * \return Zero if success, otherwise a negative error code
 *
 * Known identifiers:
 *   - _devstatus/{device}	- return status for given device
 *   - _modstatus/{modifier}	- return status for given modifier
 */
int snd_use_case_geti(snd_use_case_mgr_t *uc_mgr,
		      const char *identifier,
		      long *value);

/**
 * \brief Set new
 * \param uc_mgr Use case manager
 * \param identifier
 * \param value Value
 * \return Zero if success, otherwise a negative error code
 *
 * Known identifiers:
 *   - _fboot			- execute the fixed boot sequence (value = NULL)
 *   - _boot			- execute the boot sequence (value = NULL)
 *				   - only when driver controls identifiers are changed
 *				     (otherwise the old control values are restored)
 *   - _defaults		- execute the 'defaults' sequence (value = NULL)
 *   - _verb			- set current verb = value
 *   - _enadev			- enable given device = value
 *   - _disdev			- disable given device = value
 *   - _swdev/{old_device}	- new_device = value
 *				  - disable old_device and then enable new_device
 *				  - if old_device is not enabled just return
 *				  - check transmit sequence firstly
 *   - _enamod			- enable given modifier = value
 *   - _dismod			- disable given modifier = value
 *   - _swmod/{old_modifier}	- new_modifier = value
 *				  - disable old_modifier and then enable new_modifier
 *				  - if old_modifier is not enabled just return
 *				  - check transmit sequence firstly
 */
int snd_use_case_set(snd_use_case_mgr_t *uc_mgr,
                     const char *identifier,
                     const char *value);

/**
 * \brief Open and initialise use case core for sound card
 * \param uc_mgr Returned use case manager pointer
 * \param card_name Sound card name.
 * \return zero if success, otherwise a negative error code
 *
 * By default only first card is used when the driver card
 * name or long name is passed in the card_name argument.
 *
 * The "strict:" prefix in the card_name defines that
 * there is no driver name / long name matching. The straight
 * configuration is used.
 *
 * The "hw:" prefix in the card_name will load the configuration
 * for the ALSA card specified by the card index (value) or
 * the card string identificator.
 *
 * The sound card might be also composed from several physical
 * sound cards (for the default and strict card_name).
 * The application cannot expect that the device names will refer
 * only one ALSA sound card in this case.
 */
int snd_use_case_mgr_open(snd_use_case_mgr_t **uc_mgr,
                          const char *card_name);


/**
 * \brief Reload and re-parse use case configuration files for sound card.
 * \param uc_mgr Use case manager
 * \return zero if success, otherwise a negative error code
 */
int snd_use_case_mgr_reload(snd_use_case_mgr_t *uc_mgr);

/**
 * \brief Close use case manager
 * \param uc_mgr Use case manager
 * \return zero if success, otherwise a negative error code
 */
int snd_use_case_mgr_close(snd_use_case_mgr_t *uc_mgr);

/**
 * \brief Reset use case manager verb, device, modifier to deafult settings.
 * \param uc_mgr Use case manager
 * \return zero if success, otherwise a negative error code
 */
int snd_use_case_mgr_reset(snd_use_case_mgr_t *uc_mgr);

/*
 * helper functions
 */

/**
 * \brief Obtain a list of cards
 * \param list Returned allocated list
 * \return Number of list entries if success, otherwise a negative error code
 */
static __inline__ int snd_use_case_card_list(const char **list[])
{
	return snd_use_case_get_list(NULL, NULL, list);
}

/**
 * \brief Obtain a list of verbs
 * \param uc_mgr Use case manager
 * \param list Returned list of verbs
 * \return Number of list entries if success, otherwise a negative error code
 */
static __inline__ int snd_use_case_verb_list(snd_use_case_mgr_t *uc_mgr,
					 const char **list[])
{
	return snd_use_case_get_list(uc_mgr, "_verbs", list);
}

/**
 * \brief Parse control element identifier
 * \param elem_id Element identifier
 * \param ucm_id Use case identifier
 * \param value String value to be parsed
 * \return Zero if success, otherwise a negative error code
 */
int snd_use_case_parse_ctl_elem_id(snd_ctl_elem_id_t *dst,
				   const char *ucm_id,
				   const char *value);

/**
 * \brief Parse mixer element identifier
 * \param dst Simple mixer element identifier
 * \param ucm_id Use case identifier
 * \param value String value to be parsed
 * \return Zero if success, otherwise a negative error code
 */
int snd_use_case_parse_selem_id(snd_mixer_selem_id_t *dst,
				const char *ucm_id,
				const char *value);

/**
 *  \}
 */

#ifdef __cplusplus
}
#endif

#endif /* __ALSA_USE_CASE_H */
