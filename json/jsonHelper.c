#include "yajl_parse.h"
#include "yajl_gen.h"
#include "stringStack.h"
#include "jsonHelper.h"
//#include "Eng_String.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static struct stringList *jsonStringList;


int initList(struct stringList *list) {
	list->head = (struct stringNode *)malloc(sizeof(struct stringNode));
	if (list->head == NULL) {
		return -1;
	}
	list->head->next = NULL;
	list->rear = list->head;
	return 0;
}

int addStringNode(struct stringList *list, unsigned char *value) {
	struct stringNode *node = (struct stringNode *)malloc(sizeof(struct stringNode));
	if (node == NULL) {
		return -1;
	}
	node->value = value;
	node->next  = list->rear->next;
	list->rear->next = node;
	list->rear  = node;
	return 0;
}

unsigned char * popUpStringNode(struct stringList *list) {
// 	struct stringNode *node;
// 	unsigned char *result;
// 	if(!list)
// 		return NULL;				//lxz 20110530
// 	if (list->head == list->rear) {
// 		return NULL;
// 	}
// 	node = list->head->next;
// 	list->head->next = node->next;
// 	if (list->rear == node) {
// 		list->rear = list->head;
// 	}
// 	node->next = NULL;
// 	result = node->value;
// 	free(node);
// 	return result;
	struct stringNode *node;
	unsigned char *result = NULL;
	if(!list)
		return NULL;				//lxz 20110530
	if (list->head == list->rear) {
		return NULL;
	}
	if(list->head)
	{
		node = list->head->next;
		if(node)
		{
			list->head->next = node->next;
			if (list->rear == node) {
				list->rear = list->head;
			}
			node->next = NULL;
			result = node->value;
			free(node);
		}
	}
	return result;
}

int destoryStringList (struct stringList *list) {
	unsigned char *result;
	while((result = popUpStringNode(list)) != NULL) {

	}
	if(list && list->head)			//lxz 20110530
		free(list->head);
	if(list)						//lxz 20110708
		free(list);
	return 0;
}

void intToStr(long temp, unsigned char *result) {
 	_itoa(temp, result, 10);

	/*
	unsigned char tmp[15];
	int end, start, j;
	int index = 0;
	if (temp < 0) {
		tmp[index++] = '-';
		temp *= -1;
	}
	do {
		tmp[index++] = (temp % 10) + '0';
		temp /= 10;
	} while  (temp > 0 && index < 14);
	end = index;
	start = 0;
	if (tmp[0] == '-') {
		result[0] = '-';
		start = 1;
	}
	for (j = start; j < end; j++) {
		result[j] = tmp[index-1-j];
	}
	result[index] = '\0';
	return;
	*/
}

/* memory debugging routines */
typedef struct
{
    unsigned int numFrees;
    unsigned int numMallocs;
    /* XXX: we really need a hash table here with per-allocation
     *      information */
} yajlTestMemoryContext;

/* cast void * into context */
#define TEST_CTX(vptr) ((yajlTestMemoryContext *) (vptr))

void yajlTestFree(void * ctx, void * ptr)
{
    assert(ptr != NULL);
    TEST_CTX(ctx)->numFrees++;
    free(ptr);
}

void * yajlTestMalloc(void * ctx, unsigned int sz)
{
    assert(sz != 0);
    TEST_CTX(ctx)->numMallocs++;
    return malloc(sz);
}

void * yajlTestRealloc(void * ctx, void * ptr, unsigned int sz)
{
	void *ptr1;
    if (ptr == NULL) {
        assert(sz != 0);
        TEST_CTX(ctx)->numMallocs++;
    } else if (sz == 0) {
        TEST_CTX(ctx)->numFrees++;
    }

	if(ptr)
		ptr1 =  realloc(ptr, sz);
	else
		ptr1 = malloc(sz);
	return ptr1;
}


/* begin parsing callback routines */
#define BUF_SIZE 2048

static int test_yajl_null(void *ctx)
{
    //printf("null\n");
    return 1;
}

static int test_yajl_boolean(void * ctx, int boolVal)
{
	unsigned char *result = boolVal ? "true\0" : "false\0";
	addStringNode(jsonStringList, result);
	//printf("bool: %s\n", boolVal ? "true" : "false");
    return 1;
}

static int test_yajl_integer(void *ctx, long integerVal)
{
    
	unsigned char *result = (unsigned char *)malloc(15);
	intToStr(integerVal, result);
	addStringNode(jsonStringList, result);
    return 1;
}

static int test_yajl_double(void *ctx, double doubleVal)
{
    return 1;
}

static int test_yajl_string(void *ctx, const unsigned char * stringVal,
                            unsigned int stringLen)
{
    unsigned char *result = (unsigned char *)malloc(stringLen+1);
	memcpy(result, stringVal, stringLen);
	*(result+stringLen) = '\0';
	addStringNode(jsonStringList, result);
    return 1;
}

static int test_yajl_map_key(void *ctx, const unsigned char * stringVal,
                             unsigned int stringLen)
{
    unsigned char * str = (unsigned char *) malloc(stringLen + 1);
    str[stringLen] = 0;
    memcpy(str, stringVal, stringLen);
	addStringNode(jsonStringList, str);

    return 1;
}

static int test_yajl_start_map(void *ctx)
{
	addStringNode(jsonStringList, (unsigned char *)("{"));
    
    return 1;
}


static int test_yajl_end_map(void *ctx)
{
    addStringNode(jsonStringList, "}");

    return 1;
}

static int test_yajl_start_array(void *ctx)
{

    addStringNode(jsonStringList, "[");
    return 1;
}

static int test_yajl_end_array(void *ctx)
{
    addStringNode(jsonStringList, "]");
    
    return 1;
}

static yajl_callbacks callbacks = {
    test_yajl_null,
    test_yajl_boolean,
    test_yajl_integer,
    test_yajl_double,
    NULL,
    test_yajl_string,
    test_yajl_start_map,
    test_yajl_map_key,
    test_yajl_end_map,
    test_yajl_start_array,
    test_yajl_end_array
};

static void usage(const char * progname)
{
    fprintf(stderr,
            "usage:  %s [options] <filename>\n"
            "   -c  allow comments\n"
            "   -b  set the read buffer size\n",
            progname);
    exit(1);
}

struct stringList * parseJsonStringList(char *jsonData)
{
	yajl_handle hand;
    static unsigned char * fileData = NULL;
    unsigned int bufSize = BUF_SIZE;
    yajl_status stat;
    size_t rd;
    yajl_parser_config cfg = { 0, 1};
    int done;

	int nCount_ = 0;

    /* memory allocation debugging: allocate a structure which collects
     * statistics */
    yajlTestMemoryContext memCtx = { 0,0 };

    /* memory allocation debugging: allocate a structure which holds
     * allocation routines */
    yajl_alloc_funcs allocFuncs = {
        yajlTestMalloc,
        yajlTestRealloc,
        yajlTestFree,
        (void *) NULL
    };

    allocFuncs.ctx = (void *) &memCtx;

//	ConsolePrint(0, "parseJsonStringList start %x\n", jsonData);

    hand = yajl_alloc(&callbacks, &cfg, &allocFuncs, NULL);
	//ConsolePrint(0, "parseJsonStringList start 0\n");

	jsonStringList = (struct stringList *)malloc(sizeof(struct stringList));
	initList(jsonStringList);

	//ConsolePrint(0, "parseJsonStringList start 1\n");

    done = 0;

	while (!done) {

        if (done)
		{
            /* parse any remaining buffered data */
			//ConsolePrint(0, "parseJsonStringList start1 %d\n", nCount_++);
            stat = yajl_parse_complete(hand);	
		}
        else
		{
            /* read file data, pass to parser */
			//ConsolePrint(0, "parseJsonStringList start2 %d %x %x %d\n", nCount_++, hand, jsonData,  strlen(jsonData));
            stat = yajl_parse(hand, jsonData, strlen(jsonData));		
		}

        if (stat != yajl_status_insufficient_data &&
            stat != yajl_status_ok)
        {
            unsigned char *str = yajl_get_error(hand, 0, jsonData, rd);
            fflush(stdout);
            fprintf(stderr, "%s", (char *)str);
            yajl_free_error(hand, str);
			//ConsolePrint(0, "parseJsonStringList start3 %d\n", nCount_++);
            break;
        }
		done = 1;
    }

    yajl_free(hand);

    fflush(stderr);
    fflush(stdout);

//	{
//		struct stringNode *pNode = AK_NULL;
//		pNode = jsonStringList->head->next;
//		while(pNode)
//		{
//			AkDebugOutput("%s\r\n", pNode->value);
//			pNode = pNode->next;
//		}
//	}

    return jsonStringList;
}

struct stringList * getMap(struct stringList *list, char *key) {
	struct stringNode *node;
	struct stringNode *temp;
	unsigned char *value;
	
	struct stringList *result = NULL;    //lxz 20110530
	struct stringStack s;
	if (list == NULL) {
		return result;
	}
	
	node = list->head->next;
	while (node != NULL && strcmp(node->value, key) != 0) {
		node = node->next;
	}
	if (node != NULL && node->next != NULL) { //lxz 20110727
		
		result = (struct stringList *)malloc(sizeof(struct stringList));
		result->head = NULL; //lxz 20110530
		result->rear = NULL;

		temp = node->next;
		initList(result);
		if (strcmp(temp->value, "{") == 0 || strcmp(temp->value, "[") == 0) {
			initStack(&s);
			value = (unsigned char *)malloc(strlen(temp->value)+1);
			if(!value)
				return result;
			memcpy(value, temp->value, strlen(temp->value));
			value[strlen(temp->value)] = '\0';
			pushStack(&s, value);
			addStringNode(result, value);
			while ((temp = temp->next) != NULL) {
				value = (unsigned char *)malloc(strlen(temp->value)+1);
				if(!value)
					return result;
				memcpy(value, temp->value, strlen(temp->value));
				value[strlen(temp->value)] = '\0';
				addStringNode(result, value);
				if (strcmp(value, "{") == 0 || strcmp(value, "[") == 0) {
					pushStack(&s, value);
				} else if (strcmp(value, "}") == 0 || strcmp(value, "]") == 0) {
					popStack(&s);
				}
				if (isEmptyStack(&s) == 1) {
					break;
				}
			}
			destroyStack(&s);
		} 
		else 
		{
			value = (unsigned char *)malloc(strlen(temp->value)+1);
			if(value)
			{
				memcpy(value, temp->value, strlen(temp->value));
				value[strlen(temp->value)] = '\0';
				addStringNode(result, value);
			}
		}
	}
	return result;
}

struct stringList * getArray(struct stringList *list) {
	struct stringNode *node;
	struct stringNode *temp;
	struct stringList *result = NULL;
	unsigned char *value;
	struct stringStack s;
	if (list == NULL) {
		return result;
	}
	node = list->head->next;
	temp = node;

//	AkDebugOutput("getArray: node =  %x  ist->head = %x node->next = %x\r\n", node, list->head, node->next);

	if ((node = node->next) != NULL) {
//		AkDebugOutput("getArray: node->value =  %x %s  \r\n", node->value, node->value);

		if (strcmp(node->value, "[") == 0 || strcmp(node->value, "{") == 0) {
			initStack(&s);
			result = (struct stringList *)malloc(sizeof(struct stringList));
			initList(result);

			value = (unsigned char *)malloc(strlen(node->value)+1);
			memcpy(value, node->value, strlen(node->value));
			value[strlen(node->value)] = '\0';
			addStringNode(result, value);
			
			pushStack(&s,value);
			temp->next = node->next;
			free(node);

	//		AkDebugOutput("getArray: temp->next =  %x temp->next->next = %x \r\n", temp->next, temp->next->next);

			while ((node = temp->next) != NULL) {
				temp->next = node->next;
				value = (unsigned char *)malloc(strlen(node->value)+1);   //lxz 20110709
				memcpy(value, node->value, strlen(node->value));

	//			AkDebugOutput("getArray: value =  %s node->value = %x \r\n", value, node->value);

				value[strlen(node->value)] = '\0';
				addStringNode(result, value);
				free(node);
				if (strcmp(value, "]") == 0 || strcmp(value, "}") == 0) {
					popStack(&s);
				}
				if (isEmptyStack(&s) == 1) {
					break;
				}
			}
			destroyStack(&s);
		}
	}
//	AkDebugOutput("getArray: result =  %x \r\n", result);

	return result;
}

referType getReferType(struct stringList *p) {
	if (p != NULL) {
		if (p->head != NULL) {
			if (p->head->next != NULL) {
				if (strcmp(p->head->next->value, "[") == 0) {
					return ARRAY_TYPE;
				} else if (strcmp(p->head->next->value, "{") == 0) {
					return MAP_TYPE;
				} else {
					return NORMAL_TYPE;
				}
			}
		}
	}
	return UNKNOWN_TYPE;

}