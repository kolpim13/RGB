#pragma once

/*
 * Distribute files and requests that have been got from usb module between other modules.
 * Takes files from other module on their request if possible.
 * Should be called as often as possible
*/
void usb_task(void);