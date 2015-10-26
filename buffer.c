#include "buffer.h"

/* the buffer */
buffer_item buffer[BUFFER_SIZE
];

/* TODO buffer will also require an initialization function that initializes the
mutual-exclusion object mutex along with the empty and full semaphores.
*/

int insert_item(buffer_item item) {

/* insert item into buffer

 // TODO sync using algorithms in figure 5.9/10

return 0 if successful, otherwise
return -1 indicating an error condition */
}

int remove_item(buffer_item *item) {
/* remove an object from buffer
placing it in item

  // TODO sync using algorithms in figure 5.9/10


return 0 if successful, otherwise
return -1 indicating an error condition */
}