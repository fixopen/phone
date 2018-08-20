/*
 * jsonHelper.h
 *
 *  Created on: 2011-3-25
 *      Author: xujingcn
 */

#ifndef __JSONHELPER_H__
#define __JSONHELPER_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum referType_ {
	NORMAL_TYPE, MAP_TYPE, ARRAY_TYPE, UNKNOWN_TYPE,
}referType;


struct stringNode {
	unsigned char *value;
	struct stringNode *next;
};

struct stringList {
	struct stringNode *head;
	struct stringNode *rear;
};

referType getReferType(struct stringList *p);

struct stringList * parseJsonStringList(char *jsonData);

struct stringList * getMap(struct stringList *list, char *key);

struct stringList * getArray(struct stringList *list);

unsigned char * popUpStringNode(struct stringList *list);

int destoryStringList (struct stringList *list);

#ifdef __cplusplus
}
#endif

#endif /* GETCATEGORYLIST_H_ */
