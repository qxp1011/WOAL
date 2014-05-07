#define ACE_LACKS_CDR_ALIGNMENT 1
#define ACE_ENABLE_SWAP_ON_WRITE 1
#define ACE_CDR_IGNORE_ALIGNMENT 1
#ifdef WIN32
#include "ace/config-win32.h"
#else
#include "ace/config-linux.h"
#endif
