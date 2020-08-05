
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct c_lru_list {
	int			page;
	int			length;
	struct c_lru_list	*left;
	struct c_lru_list	*right;
	struct c_lru_list	*pos;
} c_lru_list;

c_lru_list *c_lru_list_create( void )
{
	c_lru_list *list = (c_lru_list *) malloc( sizeof(c_lru_list) );
	list->length	= 0;
	list->left	= list->right = list->pos = NULL;
	return(list);
}


bool c_lru_list_exist( c_lru_list *list, int page )
{
	bool		status	= false;
	c_lru_list	*temp	= list->right;
	printf( "当前页表项: [ " );
	while ( temp )
	{
		printf( "%d ", temp->page );
		if ( temp->page == page )
		{
			list->pos	= temp;
			status		= true;
		}
		temp = temp->right;
		if ( temp == list->right )
			break;
	}
	printf( "]" );
	return(status);
}


void c_lru_list_alloc( c_lru_list *list, int length, int page, int status )
{
	if ( status )
	{
		if ( list->pos == list->left )
		{
			list->left = list->pos->left;
			list->right = list->pos;
		}
		else if ( list->pos == list->right )
		{
		
		}
		else
		{
			list->pos->left->right	= list->pos->right;
			list->pos->right->left	= list->pos->left;

			list->pos->left		= list->left;
			list->pos->right	= list->right;

			list->left->right	= list->pos;
			list->right->left	= list->pos;

			list->right = list->pos;
		}
	}else{
		c_lru_list *node = (c_lru_list *) malloc( sizeof(c_lru_list) );

		node->page = page;
		list->length++;

		if ( list->left == NULL && list->right == NULL )
		{
			node->left	= node->right = node;
			list->left	= list->right = node;
		}else{
			node->left	= list->left;
			node->right	= list->right;

			list->left->right	= node;
			list->right->left	= node;

			list->right = node;
		}
		if ( list->length > length )
		{
			c_lru_list *temp = list->left;

			temp->left->right	= temp->right;
			temp->right->left	= temp->left;

			list->left = temp->left;
			free( temp );
		}
	}
}


int main( void )
{
	printf( "\n\n|-------------------------------------------------------|\n" );
	printf( "|******************* LRU算法模拟实验 *******************|\n" );
	printf( "|-------------------------------------------------------|\n" );

	int length = 0;
	printf( "\n\n请输入页表大小: " );
	scanf( "%d", &length );


	printf( "\n\n请输入页号序列(-1结束输入): " );
	int	page		= 0, num = 0;
	int	arr[1000]	= { 0 };
	while ( true )
	{
		scanf( "%d", &page );
		if ( page == -1 )
			break;
		arr[num++] = page;
	}

	printf( "\n\n您输入的页号序列为: " );
	for ( int idx = 0; idx < num; idx++ )
	{
		printf( "%d ", arr[idx] );
	}
	printf( "\n\n" );

	c_lru_list *list = c_lru_list_create();

	int lose = 0;
	for ( int idx = 0; idx < num; idx++ )
	{
		bool status = c_lru_list_exist( list, arr[idx] );
		c_lru_list_alloc( list, length, arr[idx], status );
		if ( status )
		{
			printf( "\n页号[%d]在页表中,不缺页\n", arr[idx] );
		}else{
			lose++;
			printf( "\n页号[%d]不在页表中,缺页\n", arr[idx] );
		}
		c_lru_list_exist( list, arr[idx] );
		printf("\n\n");
	}

	printf( "经计算,共缺[%d]页,缺页率为: [%d/%d]\n\n", lose, lose, num );

	return(0);
}