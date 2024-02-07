/** @file list.c
 *  @brief Implementation of a linked list.
 *
 * Based on the implementation approach described in "The Practice
 * of Programming" by Kernighan and Pike (Addison-Wesley, 1999).
 * 
 * @author Modified by Konrad Jasman, July 2023.
 *
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emalloc.h"
#include "list.h"
//------------------Function Prototypes
/**
 * Function:  new_node
 * -------------------
 * @brief  Allows to dynamically allocate memory for a new node to be added to the linked list.
 * This function should confirm that the argument being passed is not NULL (i.e., using the assert library). Then,
 * It dynamically allocates memory for the new node using emalloc(), and assign values to attributes associated with the node (i.e., val and next).
 * @param artist The artist to be associated with the song
 * @param song The song to be added to the list.
 * @param year The year the song was released.
 * @param score The score of the song.
 * @return node_t* A pointer to the node created.
 *
 */
node_t *new_node(char *artist, char* song, int year, char* score);
/**
 * Function:  add_front
 * --------------------
 * @brief  Allows to add a node at the front of the list.
 * @param list The list where the node will be added (i.e., a pointer to the first element in the list).
 * @param new The node to be added to the list.
 * @return node_t* A pointer to the new head of the list.
 */
node_t *add_front(node_t *list, node_t *new);
/**
 * Function:  add_end
 * ------------------
 * @brief  Allows to add a node at the end of the list.
 *
 * @param list The list where the node will be added (i.e., a pointer to the first element in the list).
 * @param new The node to be added to the list.
 *
 * @return node_t* A pointer to the head of the list.
 *
 */
node_t *add_end(node_t *list, node_t *new);
/**
 * Function:  add_inorder
 * ----------------------
 * @brief  Allows to add a new node to the list respecting an order.
 * @param list The list where the node will be added (i.e., a pointer to the first element in the list).
 * @param new The node to be added to the list.
 * @return node_t* A pointer to the node created.
 */
node_t *add_inorder(node_t *list, node_t *new);
/**
 * Function:  peek_front
 * ---------------------
 * @brief  Allows to get the head node of the list.
 *
 * @param list The list to get the node from.
 *
 * @return node_t* A pointer to the head of the list.
 *
 */
node_t *peek_front(node_t *list);
/**
 * Function:  remove_front
 * -----------------------
 * @brief  Allows removing the head node of the list.
 * @param list The list to remove the node from.
 * @return node_t* A pointer to the head of the list.
 */
node_t *remove_front(node_t *list);
/**
 * Function: apply
 * --------------
 * @brief  Allows to apply a function to the list.
 * @param list The list (i.e., pointer to head node) where the function will be applied.
 * @param fn The pointer of the function to be applied.
 * @param arg The arguments to be applied.
 */
void apply(node_t *list, void (*fn)(node_t *list, void *), void *arg);
//------------------Function Specifications
node_t *new_node(char *artist, char* song, int year, char* score){
    assert(artist != NULL); //disp error if NULL
    assert(song != NULL);


    node_t *temp = (node_t *)emalloc(sizeof(node_t));

    temp->artist = strdup(artist);
    temp->song = strdup(song);
    temp->year = year;
    temp->score = strdup(score);
    temp->next = NULL;

    return temp;
}
node_t *add_front(node_t *list, node_t *new){
    new->next = list;
    return new;
}
node_t *add_end(node_t *list, node_t *new){
    node_t *curr;

    if (list == NULL){
        new->next = NULL;
        return new;
    }

    for (curr = list; curr->next != NULL; curr = curr->next)
        ;
    curr->next = new;
    new->next = NULL;
    return list;
}
node_t *add_inorder(node_t *list, node_t *new){

    if (list == NULL){ //if list is empty, list now has new node in it
        return new;
    }
    int comparison = strcasecmp(new->score,list->score);
    if(comparison>0 || (comparison==0 && strcasecmp(new->song, list->song)<0)){ //handling for if lexicographic order dictates new song comes first
        new->next = list;
        return new;
    }

    node_t *curr = list;
    while(curr->next != NULL && (strcasecmp(curr->next->score, new->score) > 0 || (strcasecmp(curr->next->score, new->score) == 0 && strcasecmp(curr->next->song, new->song) >0))){
        curr = curr->next;
    }

    new->next = curr->next;
    curr->next = new;

    return list;
}
node_t *peek_front(node_t *list){
    return list;
}
node_t *remove_front(node_t *list){
    if (list == NULL)
    {
        return NULL;
    }

    return list->next;
}
void apply(node_t *list, void (*fn)(node_t *list, void *), void *arg){
    for (; list != NULL; list = list->next)
    {
        (*fn)(list, arg);
    }
}
