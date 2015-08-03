#ifndef _PBUFFER_H
#define _PBUFFER_H

bool pbufferInit (int width, int height);
void pbufferEnable (int width, int height);
void pbufferDisable (int width, int height);
void pbufferBind (void);
void pbufferRelease (void);
void pbufferDestroy (void);
int pbufferTexID(void);

#endif