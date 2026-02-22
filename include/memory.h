#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>

#ifndef KEYCODE_MEM_SIZE
#define KEYCODE_MEM_SIZE 1536
#endif

#ifndef IS_MCU_VERSION
#define IS_MCU_VERSION 1
#endif

#if (IS_MCU_VERSION == 0)
#include "iostream"
#endif

namespace uwu
{

typedef struct keycode_node{
    uint8_t codes[6] = {};
    uint8_t mod = 0;
    uint8_t r_id = -1;
    bool free = true;  // set when node holds valid data. If 'false' the node can be overwritten
    keycode_node* next = nullptr;
} keycode_node;

/**
 * @brief Main shared memory for keycodes. Nodes can be allocated and freed from this memory.
 *
 */
inline keycode_node keycode_mem[KEYCODE_MEM_SIZE] = {};

/**
 * @brief Get pointer to new free node. If none is found return nullptr
 *
 * @return \ref keycode_node* pointing to the newly reserved node. \ref nullptr if not possible
 */
inline keycode_node *alloc_node() {
  for (int i = 0; i < KEYCODE_MEM_SIZE; i++)
    if (keycode_mem[i].free) {
      keycode_mem[i].free = false;
      return &keycode_mem[i];
    }

  return nullptr;
}

/**
 * @brief Insert new node behind \p node in tree.
 *
 * @param node staring point for tree propagation
 * @return \ref keycode_node* pointing to the newly reserved node. \ref nullptr if not possible
 */
inline keycode_node *insert_node(keycode_node *node) {
  if (node == nullptr)
    return nullptr;

  keycode_node *next = alloc_node(); // Will return nullptr if memory full

  if (next ==
      nullptr) // If no memory allocatable, abort, but leave chain intact.
    return nullptr;

  if (node->next !=
      nullptr) // if next node not empty, append it to inserted node
    next->next = node->next;

  node->next = next;

  return next;
}

/**
 * @brief Returns the leaf node to a given tree
 *
 * @param node staring point for tree propagation
 * @return \ref keycode_node* pointing to leaf node. \ref nullptr if not found
 */
inline keycode_node *get_leaf(keycode_node *node) {
  if (node == nullptr)
    return nullptr;

  keycode_node *next = node;

  for (int i = 0; i < KEYCODE_MEM_SIZE; i++) {
    if (next->next == nullptr)
      return next;
    next = next->next;
  }

  return nullptr;
}

/**
 * @brief Append a node at the end of the tree. Any node in the tree is valid as a starting point.
 *
 * @param node staring point for tree propagation
 * @return \ref keycode_node* pointing to the newly reserved node. \ref nullptr if not possible
 */
inline keycode_node *push_back_node(keycode_node *node) {
  if (node == nullptr)
    return nullptr;

  int i = 0;
  keycode_node *next = node;
  while (next->next != nullptr) {
    if (i >= KEYCODE_MEM_SIZE)
      return nullptr;

    next = next->next;
    i++;
  }

  next->next = uwu::alloc_node();
  return next->next;
}

inline bool free_node(keycode_node* root) {
  if (root == nullptr)
    return false; // cannot free nullptr

  if (root->next != nullptr)
    free_node(root->next);

  root->next     = nullptr;
  root->free     = true;
  root->r_id     = -1;
  root->mod      = 0;
  root->codes[0] = 0;
  root->codes[1] = 0;
  root->codes[2] = 0;
  root->codes[3] = 0;
  root->codes[4] = 0;
  root->codes[5] = 0;

  return true;
}

/**
 * @brief Resets all free flags in Storage. Clears potential memory leaks.
 *
 */
inline void cleanup_chain() {
  for (int i = 0; i < KEYCODE_MEM_SIZE; i++) {
    keycode_mem[i].next     = nullptr;
    keycode_mem[i].free     = true;
    keycode_mem[i].r_id     = -1;
    keycode_mem[i].mod      = 0;
    keycode_mem[i].codes[0] = 0;
    keycode_mem[i].codes[1] = 0;
    keycode_mem[i].codes[2] = 0;
    keycode_mem[i].codes[3] = 0;
    keycode_mem[i].codes[4] = 0;
    keycode_mem[i].codes[5] = 0;
  }
}

inline void print_node(keycode_node * node)
{
#if (IS_MCU_VERSION != 0)
    return;
#else
    if(node == nullptr)
        return;

    printf("codes: ");
    for(int i = 0; i < 6; i++) printf("0x%02X ", (int)node->codes[i]);
    printf("\tmod: 0x%02X\tr_id: 0x%02X\tfree: %s\tnext: %p\n", node->mod, node->r_id, (node->free ? "true" : "false"), node->next);
#endif
}

inline void print_chain(keycode_node * node)
{
#if (IS_MCU_VERSION != 0)
    return;
#else
    if(node == nullptr)
        return;
    print_node(node);

    if(node->next != nullptr)
        print_chain(node->next);
#endif
}

inline void print_mem()
{
#if (IS_MCU_VERSION != 0)
    return;
#else
    for(int i = 0; i < KEYCODE_MEM_SIZE; i++)
    {
        std::cout << i << ": ";
        print_node(&keycode_mem[i]);
    }
#endif
}

}


#endif
