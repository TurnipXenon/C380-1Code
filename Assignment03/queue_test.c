#include "queue.h"

int main() {
    struct queue *queue = new_queue();
    
    struct mem_ref mem_ref;

    for (size_t i = 0; i < 10; i++)
    {
        
        mem_ref.address = i;
        mem_ref.page_count = i;

        enqueue(queue, mem_ref);

        if (queue->size > 3) {
            dequeue(queue);
        }
    }
    

    destroy_queue(queue);
}