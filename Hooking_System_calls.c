#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/unistd.h>
#include <asm/semaphore.h>
#include <asm/cacheflush.h>

void **sys_call_table;

asmlinkage int (*orig_call) (const char*, int, int);

asmlinkage int dummy_open_hook(const char* file, int flags, int mode)
{
   printk("A file was opened\n");
   return original_call(file, flags, mode);
}

int set_page_rw(unsigned long addr)
{
    unsigned int level;
    pte_t *pte = lookup_address(addr, &level);
    if (pte->pte &~ _PAGE_RW) pte->pte |= _PAGE_RW;
    return 0;
}

//Marking the page again readonly after doing the change
int set_page_ro(unsigned long addr)
{
    unsigned int level;
    pte_t *pte = lookup_address(addr, &level);
    pte->pte = pte->pte &~_PAGE_RW;
    return 0;
}


int init_module()
{
    // sys_call_table address in System.map
    sys_call_table = (void*)0xc061e4e0;
   
   // Preserving the original call
    orig_call = sys_call_table[__NR_open];

   //Making the sys_call_table writable as by default it is readonly.
    set_page_rw(sys_call_table);
   
   //Assigning our stub function
    sys_call_table[__NR_open] = dummy_open_hook;
}

void cleanup_module()
{
   // Restore the original call
   sys_call_table[__NR_open] = orig_call;
   set_page_ro(sys_call_table);
}


MODULE_AUTHOR("Amit Kumar");
MODULE_DESCRIPTION("A sample system call hooking module");
MODULE_LICENSE("GPL");

