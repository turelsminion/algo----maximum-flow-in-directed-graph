/*---------------------------------------------------*/
// Homework 3 : Maximum Flow (One function)
// Auther	  : Weijia Sun
// Date		  : 12/03/2014 
// Course	  : Introduction to Algorithm (22000)
// Professor  : Peter Brass
/*---------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#define INFINITY	65535
struct nodeInfo
{
	int prev;
	int curr;
};

void maximum_flow(int n, int s, int t, int *capacity, int *flow);

int main(void)
{ 
	//add main function here.
	return 0;
}

void maximum_flow(int n, int s, int t, int *capacity, int *flow)
{
	int v_num = n;
	int sPnt = s;
	int tPnt = t;

	int **capMatrix = NULL;
	int **flowMatrix = NULL;
	
	struct nodeInfo *dfs_stack_arr = NULL;	
	int dfs_stack_p;
	struct nodeInfo *dfs_visited_arr = NULL;
	int dfs_vst_p;
	struct nodeInfo *dfs_neighbor_arr = NULL;
	int dfs_nghbr_p;
	int *dfs_path_arr = NULL;
	int dfs_p_p;

	int i,j,prevNode;
	struct nodeInfo tmp_node;
	int min_value,total_flow = 0;
	char result,dfsDoneFlag,noPathFlag,errorFlag = 0;

	//*****************************************************Initialization begin
	//allocate capMatrix
	capMatrix = (int **)malloc(sizeof(int *) * v_num);
	for(i=0; i<v_num; i++)
		capMatrix[i] = (int *)malloc(sizeof(int) * v_num);
	for(i=0; i<v_num; i++)
		for(j=0; j<v_num; j++)
			capMatrix[i][j] = *(capacity + i * v_num + j);

	//allocate flowMatrix
	flowMatrix = (int **)malloc(sizeof(int *) * v_num);
	for(i=0; i<v_num; i++)
		flowMatrix[i] = (int *)malloc(sizeof(int) * v_num);
	for(i=0; i<v_num; i++)
		for(j=0; j<v_num; j++)
			flowMatrix[i][j] = 0;

	//allocate dfs_stack_arr (some vertex have many some neighbors)
	dfs_stack_arr =(struct nodeInfo *)malloc(v_num * 20 * sizeof(struct nodeInfo));
	//dfs_stack_arr =(struct nodeInfo *)malloc(v_num * sizeof(struct nodeInfo));

	//allocate dfs_visited_arr
	dfs_visited_arr =(struct nodeInfo *)malloc(v_num * sizeof(struct nodeInfo));

	//allocate dfs_neighbor_arr
	dfs_neighbor_arr =(struct nodeInfo *)malloc(v_num * sizeof(struct nodeInfo));

	//allocate dfs_path_arr
	dfs_path_arr =(int *)malloc(v_num * sizeof(int));
	//------------------------------------------------Initialization end

	//*****************************************************Do max flow begin
	noPathFlag = 0;
	while( 0 == noPathFlag )
	{
		//find the path from s to t
		dfs_stack_p = 0;
		dfs_vst_p = 0;
		dfs_nghbr_p = 0;
		dfs_stack_arr[dfs_stack_p].curr = sPnt;
		dfs_stack_arr[dfs_stack_p].prev = INFINITY;
		dfs_stack_p++;
		dfsDoneFlag = 0;
		min_value = 0;
		while( dfs_stack_p > 0 && 0 == dfsDoneFlag && 0 == errorFlag )
		{
			tmp_node.curr = dfs_stack_arr[dfs_stack_p - 1].curr;
			tmp_node.prev = dfs_stack_arr[dfs_stack_p - 1].prev;

			dfs_stack_p--;

			//check if the vertex is visited
			result = 0;
			for(i=0;i<dfs_vst_p;i++)
			{
				if( dfs_visited_arr[i].curr == tmp_node.curr )
				{
					result = 1;
					break;
				}
			}
			if( 0 == result )	// the vertex is not visited
			{
				if( dfs_vst_p < v_num )
				{
					dfs_visited_arr[dfs_vst_p].curr = tmp_node.curr;
					dfs_visited_arr[dfs_vst_p].prev = tmp_node.prev;

					if( dfs_visited_arr[dfs_vst_p].curr == tPnt )	//have been found a path from s to t
					{
						//pinpoint the real path
						dfs_p_p = 0;
						dfs_path_arr[dfs_p_p] = tPnt;
						dfs_p_p++;
						prevNode = dfs_visited_arr[dfs_vst_p].prev;
						dfs_vst_p--;
						for( i=dfs_vst_p;i>=0;i-- )
						{
							if( dfs_visited_arr[i].curr == prevNode )
							{
								dfs_path_arr[dfs_p_p] = dfs_visited_arr[i].curr;
								dfs_p_p++;
								prevNode = dfs_visited_arr[i].prev;
							}
							dfs_vst_p--;
						}

						//count the min_value
						min_value = capMatrix[dfs_path_arr[dfs_p_p-1]][dfs_path_arr[dfs_p_p-2]];
						for( i=dfs_p_p-2;i>0;i--)
							if( capMatrix[dfs_path_arr[i]][dfs_path_arr[i-1]] < min_value )
								min_value = capMatrix[dfs_path_arr[i]][dfs_path_arr[i-1]];

						//update both flowMatrix and capMatrix in local
						//update flow[][] from outside
						for( i=dfs_p_p-1;i>0;i--)
						{
							flowMatrix[dfs_path_arr[i]][dfs_path_arr[i-1]] += min_value;
							capMatrix[dfs_path_arr[i]][dfs_path_arr[i-1]] -= min_value;

							//*(flow + dfs_path_arr[i] * v_num + dfs_path_arr[i-1]) = flowMatrix[dfs_path_arr[i]][dfs_path_arr[i-1]];
						}

						total_flow += min_value;
						dfsDoneFlag = 1;
						break;
					}
					else
						dfs_vst_p++;
				}
				else
				{
					printf("Error -- dfs_visited_arr is full of data!\n");
					errorFlag = 1;
					break;
				}

				//find neighbor
				for(i=0;i<v_num;i++)
				{
					if( capMatrix[tmp_node.curr][i] > 0 )	//have capacity
					{
						if( dfs_nghbr_p < v_num )
						{
							dfs_neighbor_arr[dfs_nghbr_p].curr = i;
							dfs_neighbor_arr[dfs_nghbr_p].prev = tmp_node.curr;
							dfs_nghbr_p++;
						}
						else
						{
							printf("Error -- dfs_neighbor_arr is full of data!\n");
							errorFlag = 1;
							break;
						}
					}			
				}					

				//update dfs_stack (put elements of neighbor_arr to stack_arr if it is not visted)
				for(i=0;i<dfs_nghbr_p;i++)
				{
					result = 0;
					for(j=0;j<dfs_vst_p;j++)
					{
						if( dfs_visited_arr[j].curr == dfs_neighbor_arr[i].curr )
						{
							result = 1;
							break;
						}
					}
					if( 0 == result )
					{
						//if( dfs_stack_p < v_num )
						if( dfs_stack_p < v_num * 20 )
						{
							dfs_stack_arr[dfs_stack_p].curr = dfs_neighbor_arr[i].curr;
							dfs_stack_arr[dfs_stack_p].prev = dfs_neighbor_arr[i].prev;
							dfs_stack_p++;
						}
						else
						{
							printf("Error -- dfs_stack_arr is full of data!\n");
							errorFlag = 1;
							break;
						}
					}
				}

				//clear and prepare for the next
				dfs_nghbr_p = 0;
			}
		}

		if( 1 == dfsDoneFlag )
		{
		}
		else	//no path from s to t
		{
			noPathFlag = 1;
		}
	}
	for(i=0;i<v_num;i++)
		for(j=0;j<v_num;j++)
			*(flow + i * v_num + j) = flowMatrix[i][j];

	//----------------------------------Do max flow end

	//*****************************************************Clear begin
	//free capMatrix
	for(i=0; i<v_num; i++)
		free(capMatrix[i]);
	free(capMatrix);

	//free flowMatrix
	for(i=0; i<v_num; i++)
		free(flowMatrix[i]);
	free(flowMatrix);

	//free dfs_stack_arr
	free(dfs_stack_arr);

	//free dfs_visited_arr
	free(dfs_visited_arr);

	//free dfs_neighbor_arr
	free(dfs_neighbor_arr);

	//free dfs_path_arr
	free(dfs_path_arr);
	//-----------------------------Clear end
}