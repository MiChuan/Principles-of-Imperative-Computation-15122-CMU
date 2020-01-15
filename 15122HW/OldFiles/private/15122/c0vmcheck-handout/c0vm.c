#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#include "lib/xalloc.h"
#include "lib/stack.h"
#include "lib/contracts.h"
#include "lib/c0v_stack.h"
#include "lib/c0vm.h"
#include "lib/c0vm_c0ffi.h"
#include "lib/c0vm_abort.h"

/* call stack frames */
typedef struct frame_info frame;
struct frame_info {
  c0v_stack_t S; /* Operand stack of C0 values */
  ubyte *P;      /* Function body */
  size_t pc;     /* Program counter */
  c0_value *V;   /* The local variables */
};

int execute(struct bc0_file *bc0) {
  REQUIRES(bc0 != NULL);

  /* Variables */
  c0v_stack_t S = c0v_stack_new(); /* Operand stack of C0 values */
  /* initial program is the "main" function, function with index 0 */
  struct function_info *main_fn = &bc0->function_pool[0];
  /* Array of bytes that make up the current function */
  ubyte *P = main_fn->code;
  /* Current location within the current byte array P */
  size_t pc = 0; 
  /* Local variables (you won't need this till Task 2) */
  c0_value *V = xcalloc(main_fn->num_vars, sizeof(c0_value));
  (void) V;

  /* The call stack, a generic stack that should contain pointers to frames */
  /* You won't need this until you implement functions. */
  gstack_t callStack = stack_new();
  (void) callStack;

  while (true) {

#ifdef DEBUG
    /* You can add extra debugging information here */
    fprintf(stderr, "Opcode %x -- Stack size: %zu -- PC: %zu\n",
            P[pc], c0v_stack_size(S), pc);
#endif

    switch (P[pc]) {

    /* Additional stack operation: */

    case POP: {
      pc++;
      c0v_pop(S);
      break;
    }

    case DUP: {
      pc++;
      c0_value v = c0v_pop(S);
      c0v_push(S,v);
      c0v_push(S,v);
      break;
    }

    case SWAP: {
      pc++;
      c0_value v2 = c0v_pop(S);
      c0_value v1 = c0v_pop(S);
      c0v_push(S,v2);
      c0v_push(S,v1);
      break;
    }


    /* Returning from a function.
     * This currently has a memory leak! You will need to make a slight
     * change for the initial tasks to avoid leaking memory.  You will
     * need to be revise it further when you write INVOKESTATIC. */

    case RETURN: {
      c0_value val = c0v_pop(S);
      assert(c0v_stack_empty(S));
      c0v_stack_free(S);
      free(V);
      // Free everything before returning from the execute function!
      if (stack_empty(callStack)) {
        stack_free(callStack, NULL);
        int retval = val2int(val);
#ifdef DEBUG
      fprintf(stderr, "Returning %d from execute()\n", retval);
#endif
        return retval;
      }
      stack_elem tmp_frame_callStack = pop(callStack);
      struct frame_info *tmp_frame = (struct frame_info *)tmp_frame_callStack;
      V = tmp_frame->V;
      S = tmp_frame->S;
      P = tmp_frame->P;
      pc = tmp_frame->pc;
      free(tmp_frame);
      c0v_push(S, val);
      break;
    }


    /* Arithmetic and Logical operations */

    case IADD: {
      pc++;
      int32_t y = val2int(c0v_pop(S));
      int32_t x = val2int(c0v_pop(S));
      c0_value target = int2val(x + y);
      c0v_push(S,target);
      break;
    }

    case ISUB: {
      pc++;
      int32_t y = val2int(c0v_pop(S));
      int32_t x = val2int(c0v_pop(S));
      c0_value target = int2val(x - y);
      c0v_push(S,target);
      break;
    }

    case IMUL: {
      pc++;
      int32_t y = val2int(c0v_pop(S));
      int32_t x = val2int(c0v_pop(S));
      c0_value target = int2val(x * y);
      c0v_push(S,target);
      break;
    }

    case IDIV: {
      pc++;
      int32_t y = val2int(c0v_pop(S));
      int32_t x = val2int(c0v_pop(S));
      if (y == 0) c0_arith_error("idiv error");
      else if (x == INT32_MIN && y == -1) 
              c0_arith_error("idiv error");
      c0_value target = int2val(x / y);
      c0v_push(S,target);
      break;
    }

    case IREM: {
      pc++;
      int32_t y = val2int(c0v_pop(S));
      int32_t x = val2int(c0v_pop(S));
      if (y == 0) c0_arith_error("irem error");
      else if (x == INT32_MIN && y == -1) 
               c0_arith_error("irem error");
      c0_value target = int2val(x % y);
      c0v_push(S,target);
      break;
    }

    case IAND: {
      pc++;
      int32_t y = val2int(c0v_pop(S));
      int32_t x = val2int(c0v_pop(S));
      c0_value target = int2val(x & y);
      c0v_push(S,target);
      break;
    }

    case IOR: {
      pc++;
      int32_t y = val2int(c0v_pop(S));
      int32_t x = val2int(c0v_pop(S));
      c0_value target = int2val(x | y);
      c0v_push(S,target);
      break;
    }

    case IXOR: {
      pc++;
      int32_t y = val2int(c0v_pop(S));
      int32_t x = val2int(c0v_pop(S));
      c0_value target = int2val(x ^ y);
      c0v_push(S,target);
      break;
    }

    case ISHL: {
      pc++;
      int32_t y = val2int(c0v_pop(S));
      int32_t x = val2int(c0v_pop(S));
      if (0 <= y && y <= 31) {
        c0_value target = int2val(x << y);
        c0v_push(S,target);
        break;
      }
      c0_arith_error("ishl error");
      break;
    }

    case ISHR: {
      pc++;
      int32_t y = val2int(c0v_pop(S));
      int32_t x = val2int(c0v_pop(S));
      if (0 <= y && y <= 31) {
        c0_value target = int2val(x >> y);
        c0v_push(S,target);
        break;
      }
      c0_arith_error("ishr error");
      break;
    }


    /* Pushing constants */

    case BIPUSH: {
      pc++;
      int32_t x = (int32_t)(int8_t)P[pc];
      c0v_push(S, int2val(x));
      pc++;
      break;
    }

    case ILDC: {
      pc++;
      uint32_t c1 = (uint32_t)P[pc];
      pc++;
      uint32_t c2 = (uint32_t)P[pc];
      int32_t x = (int)bc0->int_pool[(c1 << 8) | c2];
      c0v_push(S, int2val(x));
      pc++;
      break;
    }

    case ALDC: {
      pc++;
      uint32_t c1 = (uint32_t)P[pc];
      pc++;
      uint32_t c2 = (uint32_t)P[pc];
      void *a = &((bc0->string_pool)[(c1 << 8) | c2]);
      c0v_push(S, ptr2val(a));
      pc++;
      break;
    }


    case ACONST_NULL: {
      pc++;
      c0v_push(S, ptr2val(NULL));
      break;
    }


    /* Operations on local variables */

    case VLOAD: {
      uint8_t i = P[pc + 1];
      pc+=2;
      c0v_push(S, V[i]); 
      break;
    }

    case VSTORE: {
      uint8_t i = P[pc + 1];
      pc+=2;
      V[i] = c0v_pop(S);
      break;
    }


    /* Control flow operations */

    case NOP: {
      pc++;
      break;
    }

    case IF_CMPEQ: {
      c0_value v2 = c0v_pop(S);
      c0_value v1 = c0v_pop(S);
      if (val_equal(v1, v2)) {
        uint8_t o1 = P[pc + 1];
        uint8_t o2 = P[pc + 2];
        int16_t offset = (int16_t)(((uint16_t)o1) << 8 | ((uint16_t)o2));
        pc += offset;
      }
      else {
        pc+=3;
      }
      break;
    }

    case IF_CMPNE: {
      c0_value v2 = c0v_pop(S);
      c0_value v1 = c0v_pop(S);
      if (!val_equal(v1, v2)) {
        uint8_t o1 = P[pc + 1];
        uint8_t o2 = P[pc + 2];
        int16_t offset = (int16_t)(((uint16_t)o1) << 8 | ((uint16_t)o2));
        pc += offset;
      }
      else {
        pc+=3;
      }
      break;
    }

    case IF_ICMPLT: {
      int32_t y = val2int(c0v_pop(S));
      int32_t x = val2int(c0v_pop(S));
      if (x < y) {
        uint8_t o1 = P[pc + 1];
        uint8_t o2 = P[pc + 2];
        int16_t offset = (int16_t)(((uint16_t)o1) << 8 | ((uint16_t)o2));
        pc += offset;
      }
      else {
        pc+=3;
      }
      break;
    }

    case IF_ICMPGE: {
      int32_t y = val2int(c0v_pop(S));
      int32_t x = val2int(c0v_pop(S));
      if (x >= y) {
        uint8_t o1 = P[pc + 1];
        uint8_t o2 = P[pc + 2];
        int16_t offset = (int16_t)(((uint16_t)o1) << 8 | ((uint16_t)o2));
        pc += offset;
      }
      else {
        pc+=3;
      }
      break;
    }

    case IF_ICMPGT: {
      int32_t y = val2int(c0v_pop(S));
      int32_t x = val2int(c0v_pop(S));
      if (x > y) {
        uint8_t o1 = P[pc + 1];
        uint8_t o2 = P[pc + 2];
        int16_t offset = (int16_t)(((uint16_t)o1) << 8 | ((uint16_t)o2));
        pc += offset;
      }
      else {
        pc+=3;
      }
      break;
    }

    case IF_ICMPLE: {
      int32_t y = val2int(c0v_pop(S));
      int32_t x = val2int(c0v_pop(S));
      if (x <= y) {
        uint8_t o1 = P[pc + 1];
        uint8_t o2 = P[pc + 2];
        int16_t offset = ((int16_t)(uint16_t)o1) << 8 | ((int16_t)(uint16_t)o2);
        pc += offset;
      }
      else {
        pc+=3;
      }
      break;
    }

    case GOTO: {
      uint8_t o1 = P[pc + 1];
      uint8_t o2 = P[pc + 2];
      int16_t offset = (int16_t)(((uint16_t)o1) << 8 | ((uint16_t)o2));
      pc += offset;
      break;
    }

    case ATHROW: {
      void *a = val2ptr(c0v_pop(S));
      char *s = (char *)a;
      c0_user_error(s);
      pc++;
      break;
    }

    case ASSERT: {
      void *a = val2ptr(c0v_pop(S));
      char *s = (char *)a;
      int32_t x = val2int(c0v_pop(S));
      if (x == 0) {
        c0_assertion_failure(s);
      }
      pc++;
      break;
    }


    /* Function call operations: */

    case INVOKESTATIC: {
      frame *tmp_frame = xmalloc(sizeof(frame));
      tmp_frame->S = S;
      tmp_frame->P = P;
      tmp_frame->pc = pc + 3;
      tmp_frame->V = V;
      push(callStack, (void *)tmp_frame);
      uint8_t c1 = P[pc + 1];
      uint8_t c2 = P[pc + 2];
      uint16_t offset = ((uint16_t)c1 << 8 | (uint16_t)c2);
      struct function_info *new_fn_g = &bc0->function_pool[offset];
      V = xcalloc(new_fn_g->num_vars, sizeof(c0_value));
      //initalize a new array of local variables 
      //from old operand stack
      for (int i = new_fn_g->num_args - 1; i >= 0; i--) {
        V[i] = c0v_pop(tmp_frame->S);
      }
      S = c0v_stack_new();
      P = new_fn_g->code;
      pc = 0;
      break;
    }

    case INVOKENATIVE: {
      uint8_t c1 = P[pc + 1];
      uint8_t c2 = P[pc + 2];
      uint16_t offset = ((uint16_t)c1 << 8 | (uint16_t)c2);
      struct native_info *native_fn_info = &bc0->native_pool[offset];
      c0_value *arr = xcalloc(native_fn_info->num_args ,sizeof(c0_value));
      for (int i = native_fn_info->num_args - 1; i >= 0; i--) {
        arr[i] = c0v_pop(S);
      }
      native_fn *nati_fn = 
      native_function_table[native_fn_info->function_table_index];
      c0_value res = (*nati_fn)(arr);
      c0v_push(S, res);
      free(arr);
      pc+=3;
      break;
    }

    /* Memory allocation operations: */

    case NEW: {
      uint8_t s = P[pc + 1];
      void *a = xcalloc(1, s);
      c0v_push(S, ptr2val(a));
      pc+=2;
      break;
    }

    case NEWARRAY: {
      uint8_t s = P[pc + 1];
      int32_t n = val2int(c0v_pop(S));
      c0_array *a = xcalloc(1, sizeof(c0_array));
      a->count = n;
      a->elt_size = s;
      a->elems = xcalloc(n, s);
      c0v_push(S, ptr2val((void *)a));
      pc+=2;
      break;
    }

    case ARRAYLENGTH: {
      void *a = val2ptr(c0v_pop(S));
      c0_array *c0_arr = (c0_array *)a;
      int length = c0_arr->count;
      c0v_push(S, int2val(length));
      pc++;
      break;
    }


    /* Memory access operations: */

    case AADDF: {
      char *a = (char *)val2ptr(c0v_pop(S));
      uint8_t f = P[pc + 1];
      if (a == NULL) c0_memory_error("Memory Error!");
      c0v_push(S, ptr2val((void *)(a + f)));
      pc+=2;
      break;
    }

    case AADDS: {
      pc++;
      int32_t i = val2int(c0v_pop(S));
      void *a = val2ptr(c0v_pop(S));
      c0_array *c0_arr = (c0_array *)a;
      if (c0_arr == NULL) {
        c0_memory_error("Memory Error!");
      }
      if (!(0 <= i && i < c0_arr->count)) {
        c0_memory_error("Memory Error!");
      }
      char *address = &(((char *)(c0_arr->elems))[c0_arr->elt_size * i]);
      c0v_push(S, ptr2val((void *)address));
      break;
    }

    case IMLOAD: {
      int *a = (int *)val2ptr(c0v_pop(S));
      if (a == NULL) c0_memory_error("Memory Error!");
      int32_t x = *a;
      c0v_push(S, int2val(x));
      pc++;
      break;
    }

    case IMSTORE: {
      int32_t x = val2int(c0v_pop(S));
      int *a = (int *)val2ptr(c0v_pop(S));
      if (a == NULL) c0_memory_error("Memory Error!");
      *a = x;
      pc++;
      break;
    }

    case AMLOAD: {
      // a is void** which is a pointer to a (void *)
      void **a = val2ptr(c0v_pop(S)); 
      if (a == NULL) c0_memory_error("Memory Error!");
      void *b = *a;
      c0v_push(S, ptr2val(b));
      pc++;
      break;
    }

    case AMSTORE: {
      // a is void** which is a pointer to a (void *)
      void *b = val2ptr(c0v_pop(S));
      void **a = val2ptr(c0v_pop(S));
      if (a == NULL) c0_memory_error("Memory Error!");
      *a = b;
      pc++;
      break;
    }

    case CMLOAD: {
      char *a = (char *)val2ptr(c0v_pop(S));
      if (a == NULL) c0_memory_error("Memory Error!");
      int32_t x = (int32_t)*a;
      c0v_push(S, int2val(x));
      pc++;
      break;
    }

    case CMSTORE: {
      int32_t x = val2int(c0v_pop(S));
      char *a = (char *)val2ptr(c0v_pop(S));
      if (a == NULL) c0_memory_error("Memory Error!");
      *a = (x & 0x7f);
      pc++;
      break;
    }

    default:
      fprintf(stderr, "invalid opcode: 0x%02x\n", P[pc]);
      abort();
    }
  }

  /* cannot get here from infinite loop */
  assert(false);
}


