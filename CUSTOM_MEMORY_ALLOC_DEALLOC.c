#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/mempool.h>
#define STR_POOL_SIZE 5

mempool_t *string_pool;

void *string_alloc(gfp_t gfp_mask, void *pool_data)
{
    pr_notice("string_alloc called\n");
    return kzalloc(8192, gfp_mask);
    
}

void string_free(void *element, void *pool_data)
{
    pr_notice("string_free called\n");
    kfree(element);
}

static int __int mempool_test_init(void)
{
    char *ptr1, *ptr2, *ptr3;
    
    string_pool = mempool_create(STR_POOL_SIZE, string_alloc, string_free, NULL);
    pr_notice("Allocated memory pool (%p) of size:%d\n", (void*)string_pool, 5);
    
    ptr1 = (char*)mempool_alloc(string_pool, GFP_KERNEL);
    pr_notice("Allocated buffer :ptr1=%p\n", ptr1);
    
    
    ptr2 = (char*)mempool_alloc(string_pool, GFP_KERNEL);
    pr_notice("Allocated buffer :ptr1=%p\n", ptr2);
    
    mempool_free(ptr1, string_pool);
    pr_notice("Freed buffer:ptr1\n");
    
    
    mempool_free(ptr2, string_pool);
    pr_notice("Freed buffer:ptr1\n");
    
    retrun 0;
    
    
}

static void __exit mempool_test_exit(void)
{
    pr_notice("\n Destroying memory pool:%p\n", (void*)string_pool);
    mempool_destroy(string_pool);
    retrun ;
}

module__init(mempool_test_init);
module__exit(mempool_test_exit);
MODULE_LICENSE("GPL");
