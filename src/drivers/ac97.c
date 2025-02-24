#include "ac97.h"

struct audio_driver *ac97_init(struct PCI_driver *driver){
	struct audio_driver *audio = malloc(sizeof(struct audio_driver));
	audio->pci = driver;
	return audio;
}