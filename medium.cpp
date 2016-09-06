#include <iostream>

#include <algorithm>
#include <cmath>

#include <array>
#include <bitset>
#include <list>
#include <string>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <vector>

using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

/* 2. Add Two Numbers */
ListNode* addTwoNumbers(ListNode* l1, ListNode* l2);
/* 3. Longest Substring Without Repeating Characters */
int lengthOfLongestSubstring(string s);
/* 5. Longest Palindromic Substring */
string longestPalindrome(string s);
size_t longestOddPalindrome(string s, size_t i);
size_t longestEvenPalindrome(string s, size_t i);
/* 11. */
int maxWidth(vector<size_t> &lines, list<pair<size_t, size_t> > &containers);
/* 31. Next Permutation */
void flipOrder(vector<int> &nums, int offset);
void nextPermutation(vector<int> &nums);
/* 62. Unique Paths */
int uniquePaths(int m, int n);
/* 63. Unique Paths II */
int uniquePathsWithObstacles(vector<vector<int> >& obstacle_grid);
/* 94. Binary Tree Inorder Traversal (iterative) */
vector<int> inorderTraversal(TreeNode *root);
/* 95. Unique Binary Search Trees II */
vector<TreeNode*> generateTrees(int n);
vector<TreeNode*> generateTrees(int lowestNum, int highestNum);
/* 96. Unique Binary Search Trees */
int numTrees(int n);
/* 201. Bitwise AND of Numbers Range */
int rangeBitwiseAnd(int m, int n);
/* 268. Missing Number */
int missingNumber(vector<int> &nums);
/* 328. Odd Even Linked List */
ListNode *oddEvenList(ListNode *head);
/* 338. Counting Bits */
vector<int> countBits(int num);

/* 2. Add Two Numbers
 *
 * Description: Take two singly-linked lists each representing numbers as input. Return a 
 * 				singly-linked list that represents the sum of the input lists.
 * url: 		https://leetcode.com/problems/add-two-numbers/
 */
ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
	if (l1 == nullptr && l2 == nullptr) {
		return nullptr;
	}

	ListNode *head_ptr = new ListNode(0);		// the node whose next member is to be returned
	ListNode *current = head_ptr;				// the node used to generate the return list

	int remainder = 0;

	// while either (or both) of l1, l2 have remaining numbers to add to the return list, add the 
	// required numbers
	while (l1 != nullptr || l2 != nullptr) {
		current->next = new ListNode(0);
		current = current->next;

		// add the remainder from the previous sum to the value
		current->val += remainder;

		// if l1 still contains numbers, add the next number to current->val
		if (l1 != nullptr) {
			current->val += l1->val;
			l1 = l1->next;
		}

		// if l2 still contains numbers, add the next number to current->val
		if (l2 != nullptr) {
			current->val += l2->val;
			l2 = l2->next;
		}

		// if current->val is not a single-digit number, store the remainder and remove it
		if (current->val > 9) {
			remainder = current->val / 10;
			current->val -= remainder * 10;
		} else {
			remainder = 0;
		}
	}

	if (remainder > 0) {
		current->next = new ListNode(remainder);
	}

	// create the list to be returned from the pointer contained in head, delete the head node
	ListNode *return_list = head_ptr->next;
	delete head_ptr;

	return return_list;
}

/* 3. Longest Substring Without Repeating Characters
 *
 * Description: Given a string as input, return the length of the longest substring with no
 * 				repeating letters.
 * url: 		https://leetcode.com/problems/longest-substring-without-repeating-characters/
 */
int lengthOfLongestSubstring(string s) {
	if (s.length() <= 1) {
		return s.length();
	}

	int longest_run_found = 1;

	// the index at the beginning of the current run (usually the index after the previous char 
	// repeptition found)
	size_t current_run_start = 0;			
	
	// stores the index that each char previously occurred at
	vector<size_t> previous_index;	

	// because size_t is unsigned, assign all places to 0 and keep track of the first char in the
	// string so any values of 0 in the previous_index vector can be validated
	previous_index.assign(128, 0);
	char first_char = s.at(0);

	// iterate through all chars in s
	char c;
	for (size_t i = 1; i < s.length(); i++) {
		c = s.at(i);

		// if c occurred previously in the current run and c's previous_index is not invalid
		// (initially-set 0), then our current run has been broken
		if (previous_index[c] >= current_run_start &&
				!(previous_index[c] == 0 && c != first_char)) {
			// check for new longest_run
			longest_run_found = max(longest_run_found, (int)(i - current_run_start));

			// reset current_run_start to after the previous occurrence of c
			current_run_start = previous_index[c] + 1;
		}

		previous_index[c] = i;
	}

	return max(longest_run_found, (int)(s.length() - current_run_start));
}

/* 5. Longest Palindromic Substring
 *
 * Description: Find and return the longest palindromic substring of s.
 * url:			https://leetcode.com/problems/longest-palindromic-substring/
 */
string longestPalindrome(string s) {
	if (s.length() <= 1) return s;

	// both int arrays store the palindrome length, begin index, and string length of palindromes
	// located in s
	size_t longest_pal_index = 0;
	size_t longest_pal_len = 1;
	size_t pal_length; 

	for (size_t i = 0; i < s.length() - (longest_pal_len / 2); i++) {
		if ((pal_length = longestOddPalindrome(s, i)) > longest_pal_len) {
			// store new longest palindrome index and length
			longest_pal_index = i - (pal_length / 2);
			longest_pal_len = pal_length;
		}

		if ((pal_length = longestEvenPalindrome(s, i)) > longest_pal_len) {
			// store new longest palindrome index and length
			longest_pal_index = i - ((pal_length - 1) / 2);
			longest_pal_len = pal_length;
		}
	}

	return s.substr(longest_pal_index, longest_pal_len);
}

size_t longestOddPalindrome(string s, size_t i) {
	size_t start = i;
	size_t end = i;

	while (start > 0 && end < s.length() - 1) {
		if (tolower(s.at(--start)) != tolower(s.at(++end))) {
			start++;
			end--;
			break;
		}
	}

	return end - start + 1;
}


size_t longestEvenPalindrome(string s, size_t i) {
	if (tolower(s.at(i)) != tolower(s.at(i + 1))) {
		return 0;
	}

	size_t start = i;
	size_t end = i + 1;

	while (start > 0 && end < s.length() - 1) {
		if (tolower(s.at(--start)) != tolower(s.at(++end))) {
			start++;
			end--;
			break;
		}
	}

	return end - start + 1;
}

/* 11. Container with most Water
 *
 */
int maxArea(vector<int> &height) {
	// for each height, height_indexes maps a vector containing all indexes of lines with this 
	// height
	unordered_map<int, vector<size_t> > height_indexes;
	int max_height = 0;		// the height of the longest line

	// add all lines in height to height_indexes
	for (size_t i = 0; i < height.size(); i++) {
		height_indexes[height[i]].push_back(i);
		max_height = max(max_height, height[i]);
	}

	// a list of all possible containers of the current height. start and end index of each
	// container are stored. begins with a single container (of height 0) with the max possible
	// width.
	list<pair<size_t, size_t> > containers;
	containers.emplace_front(0, height.size() - 1);
	
	// vector containing the indexes of all new lines which would separate the containers at the 
	// current height (ie. when we move from height 1 to height 2, all lines of height 1 may
	// "separate" them into two containers and therefore will be stored in lines)
	vector<size_t> lines;
	
	int max_width = height.size() - 1;
	int max_area = 0;	// the area of the largest-area container found so far

	// loop through all possible line heights
	for (int height = 1; height <= max_height; height++) {
		lines = height_indexes[height - 1];

		if (!lines.empty()) {
			// if lines of the current height exist, use maxWidth to split all required containers 
			// and to return the width of the widest container that exists after all required splits
			// performed.
			max_width = maxWidth(lines, containers);
		}

		max_area = max(max_area, height * max_width);
	}

	return max_area;
}

int maxWidth(vector<size_t> &lines, list<pair<size_t, size_t> > &containers) {
	int max_width = 0;
	auto lines_it = lines.begin();	// iterator for indexes of lines that could split the current
									// containers
	// iterate all containers, checking first if they need to be separated and secondly if their
	// area is larger than max_area
	for (auto ctr_it = containers.begin(); ctr_it != containers.end();) {
		// attempt to find the first index of a line that is >= the beginning index of the current
		// container
		while (lines_it != lines.end() && *lines_it < (*ctr_it).first) {
			lines_it++;
		}

		if (lines_it != lines.end() && *lines_it <= (*ctr_it).second) {
			// if the line index is within the container, split it into 0-2 containers
			auto line_index = *lines_it;
			vector<pair<size_t, size_t> > replacement_containers;

			if (line_index != 0 && line_index - (*ctr_it).first > 1) {
				replacement_containers.push_back(make_pair((*ctr_it).first, line_index - 1));
			}

			if ((*ctr_it).second - line_index > 1) {
				replacement_containers.push_back(make_pair(line_index + 1, (*ctr_it).second));
			}

			switch (replacement_containers.size()) {
				case 0:
					{
						auto ctr_to_remove = ctr_it;
						ctr_it++;
						containers.erase(ctr_to_remove);
						break;
					}
				case 1:
					*ctr_it = replacement_containers[0];
					max_width = max(max_width, (int)((*ctr_it).second - (*ctr_it).first));
					ctr_it++;
					break;
				case 2:
					max_width = max(max_width, (int)(replacement_containers[0].second - 
								replacement_containers[0].first) + 1);
					containers.insert(ctr_it, replacement_containers[0]);
					*ctr_it = replacement_containers[1];
			}
		} else {
			// if the line index is not within the container, check for a new max_width and iterate
			max_width = max(max_width, (int)((*ctr_it).second - (*ctr_it).first));
			ctr_it++;
		}
	}

	max_width = max(max_width, (int)(containers.back().second - containers.back().first));

	return max_width;
}

/* 31. Next Permutation
 *
 * Description: An input vector is to be treated as a number that needs to be increased to the next
 * 				highest value which uses all of the same digits. If the number is the highest
 * 				possible arrangement of digits, it needs to be changed to the lowest possible
 * 				arrangement.
 * url: 		https://leetcode.com/problems/next-permutation/
 */
void nextPermutation(vector<int> &nums) {
	if (nums.size() <= 1) {
		return;
	}

	// Find the first number that needs to be swapped. This is the first number, iterating right to
	// left, that is smaller than the previous (right-hand-side) number.
	int first_swap_index;
	for (first_swap_index = nums.size() - 2; first_swap_index >= 0; first_swap_index--) {
		if (nums[first_swap_index] < nums[first_swap_index + 1]) {
			break;
		}
	}

	// If we have found a suitable first number to swap, find second_swap_index and perform the 
	// swap. second_swap_index will be the index of the first number (iterating right to left) that
	// is larger than the number at first_swap_index.
	if (first_swap_index >= 0) {
		// find second_swap_index
		int second_swap_index;
		for (second_swap_index = nums.size() - 1; second_swap_index > first_swap_index; 
				second_swap_index--) {
			if (nums[second_swap_index] > nums[first_swap_index]) {
				break;
			}
		}

		// perform the swap
		int temp = nums[first_swap_index];
		nums[first_swap_index] = nums[second_swap_index];
		nums[second_swap_index] = temp;
	}

	// flip the order of all numbers following first_swap_index. If a swap was not performed, this
	// flip still needs to be performed from index 0 onwards (which should be first_swap_index + 1)
	flipOrder(nums, first_swap_index + 1);
}

void flipOrder(vector<int> &nums, int offset) {
	// swapping needs to occur between indexes offset and loop_max inclusive. Each num is swapped
	// with it's counterpart further right in the vector
	// eg: [3, 8, 7, 3, 2, 2, 1] offset = index of the number 8, loop_max = index of 3
	int loop_max = (nums.size() - offset - 1) / 2;
	int temp;

	for (int i = 0; i <= loop_max; i++) {
		temp = nums[offset + i];
		nums[offset + i] = nums[nums.size() - 1 - i];
		nums[nums.size() - 1 - i] = temp;
	}
}

/* 62. Unique Paths
 *
 * Description: Return the number of unique paths that can be taken from the top left place to the
 * 				bottom right place in an n x m grid. Only right and downward movements are allowed.
 * url: 		https://leetcode.com/problems/unique-paths/
 */
int uniquePaths(int m, int n) {
	// Two-rowed vector storing the number of ways to uniquely traverse to each node. As the
	// solution is calculated, the current_row is toggled and calculated from the non-current row.
	vector<vector<int> > paths_ct(2, vector<int>(n));

	// toggled between 0 and 1 to indicate the current row being calculated
	bool current_row = false;

	for (int j = 0; j < n; j++) {
		paths_ct[current_row][j] = 1;
	}

	// Loop through the remaining rows. Each iteration toggles current_row, sets the first element
	// in the vector's current row to 1 then proceeds to calculate the remaining numbers in the
	// current row.
	for (int i = 1; i < m; i++) {
		current_row = !current_row;
		paths_ct[current_row][0] = 1;

		for (int j = 1; j < n; j++) {
			paths_ct[current_row][j] = paths_ct[!current_row][j] + paths_ct[current_row][j-1];
		}
	}

	return paths_ct[current_row][n - 1];
}

/* 63. Unique Paths II 
 * Description: Return the number of unique paths that can be taken from the top left place to the
 * 				bottom right place in obstacle_grid. Obstacles in obstacle_grid are denoted with
 * 				the number 1 and can't be moved through by any path. Only right and downward
 * 				movements are allowed.
 * url: https://leetcode.com/problems/unique-paths-ii/
 */
int uniquePathsWithObstacles(vector<vector<int> >& obstacle_grid) {
	// this problem is solved in place by assigning a value representing the number of unique paths
	// to each node in obstacle_grid, to the node itself. Negative numbers are used instead of
	// positive numbers to avoid replacing all obstacles in obstacle_grid with negative numbers at
	// the beginning of the method.
	
	int height = obstacle_grid.size();
	int width = obstacle_grid[0].size();

	// if the top left cell is 0, set it to -1, otherwise we can't complete any paths
	if (obstacle_grid[0][0] != 1 && obstacle_grid[height - 1][width - 1] != 1) {
		obstacle_grid[0][0] = -1;
	} else {
		return 0;
	}

	// populate the top row with the correct values
	for (int j = 1; j < width; j++) {
		if (obstacle_grid[0][j] == 0) {
			// if we haven't hit an obstacle yet, set the current cell to -1
			obstacle_grid[0][j] = -1;
		} else if (obstacle_grid[0][j] == 1) {
			// if the current cell is an obstacle, break
			break;
		}
	}

	// populate the remaining cells
	for (int i = 1; i < height; i++) {
		// set the first element of the row to -1 if required
		if (obstacle_grid[i][0] == 0 && obstacle_grid[i - 1][0] == -1) {
			obstacle_grid[i][0] = -1;
		}

		// set the remaining elements in the row
		for (int j = 1; j < width; j++) {
			if (obstacle_grid[i][j] != 1) {
				// if the current element isn't 1, add above and left cells if required
				if (obstacle_grid[i - 1][j] < 0) {
					// add above element if it is under 0
					obstacle_grid[i][j] += obstacle_grid[i - 1][j];
				}
				if (obstacle_grid[i][j - 1] < 0) {
					// add left element if it is under 0
					obstacle_grid[i][j] += obstacle_grid[i][j - 1];
				}
			}
		}
	}
	
	// return bottom right value (multiplied by -1 to make it positive)
	return obstacle_grid[height - 1][width - 1] * -1;
}

/* 94. Binary Tree Inorder Traversal (iterative implementation)
 *
 * Description: Return all numbers in a BST generated by an Inorder Traversal. Made more difficult
 * 				by the fact that nodes don't store parent refernces and that an iterative
 * 				implementation was preferred.
 * url: 		https://leetcode.com/problems/binary-tree-inorder-traversal/
 */
vector<int> inorderTraversal(TreeNode *root) {
	vector<int> return_vals;
	
	if (root == nullptr) {
		return return_vals;
	}

	// a stack of all parent nodes to the current node being processed
	stack<TreeNode *> parent_stack;
	parent_stack.push(root);

	TreeNode *prev_node;		// the previous node that was processed
	TreeNode *current;			// the current node being processed

	while (!parent_stack.empty()) {
		current = parent_stack.top();

		if (current->left == nullptr && current->right == nullptr) {
			// if we're a node with no children, add self to return vals and remove self from stack
			return_vals.push_back(current->val);
			prev_node = current;
			parent_stack.pop();
		} else if (current->left != nullptr && prev_node != current->left 
				&& prev_node != current->right) {
			// if left hasn't been visited, just add left to the stack so we go there in the next 
			// iteration
			parent_stack.push(current->left);
		} else {
			// we're at a node that doesn't need to go left, what we need to do depends on whether
			// we need to visit right, it doesn't exist, or if we have already visited it
			if (current->right != nullptr && prev_node != current->right) {
				// right exists and hasn't been visited
				return_vals.push_back(current->val);
				prev_node = current;
				parent_stack.push(current->right);
			} else if (current->right == nullptr) {
				// right doesn't exist
				return_vals.push_back(current->val);
				prev_node = current;
				parent_stack.pop();
			} else {
				// right exists and has been visited
				prev_node = current;
				parent_stack.pop();
			}
		}
	}

	return return_vals;
}

/* 95. Unique Binary Search Trees II 
 *
 * Description: Return all structurally unique BSTs which contain all values 1...n
 * url: 		https://leetcode.com/problems/unique-binary-search-trees-ii/
 */
vector<TreeNode*> generateTrees(int n) {
	if (n - 1 <= -1) {
		vector<TreeNode*> node;
		return node;
	} else {
		return generateTrees(1, n);
	}
}

vector<TreeNode*> generateTrees(int lowestNum, int highestNum) {
	vector<TreeNode*> treeList;
	
	if (highestNum - lowestNum == -1) {
		// we need to add a single null node to treeList
		treeList.push_back(nullptr);
	} else if (highestNum - lowestNum == 0) {       
		// we need to add a single nonnull node to treeList
		TreeNode* node = new TreeNode(lowestNum);
		treeList.push_back(node);
	} else {
		// we need to add multiple nodes to treeList
		// iterate each possible root node value (from lowestNum to highestNum)
		for (int i = lowestNum; i <= highestNum; i++) {
			// generate lists of possible left and right nodes
			vector<TreeNode*> left_nodes = generateTrees(lowestNum, i - 1);
			vector<TreeNode*> right_nodes = generateTrees(i + 1, highestNum);
		
			// for each possible left node, pair it with each possible right node as a new node
			// with value i and append it to treeList
			for (auto it_left = left_nodes.begin(); it_left != left_nodes.end(); it_left++) {
				for (auto it_right = right_nodes.begin(); it_right != right_nodes.end(); 
						it_right++) {
					TreeNode* node = new TreeNode(i);
					node->left = *it_left;
					node->right = *it_right;
					treeList.push_back(node);
				}
			}
		}
	}
	
	return treeList;
}

/* 96. Unique Binary Search Trees
 *
 * Description: Return the amount of structuarally unique BSTs that store all values 1...n
 * url: 		https://leetcode.com/problems/unique-binary-search-trees/
 */
int numTrees(int n) {
	// the solutions for n = 0 to n = 4
    int solutions[5] = {1, 1, 2, 5, 12};

	if (n <= 2) {
		return solutions[n];
	} else {
		int total = 0;

		// accumulate the number of permutations when each of the numbers 1...n/2 are the root
		for (int i = 1; i <= n / 2; i++) {
			total += numTrees(i - 1) * numTrees(n - i);
		}
		// multiply by two, to account for the numbers from middle...n
		total *= 2;

		// if n is odd, add the permutations when the middle number is the root
		if (n % 2 == 1) {
			// find the number of permutations for the left branch of the tree
			int left_branch_permutations = numTrees(n / 2);
			// square left_branch_permuatations because there are an equal amount on the right
			total += left_branch_permutations * left_branch_permutations;
		}

		return total;
	}
}


/* 201. Bitwise AND of Numbers Range
 *
 * Description: Return the result of a bitwise and operation on all numbers in the range m...n.
 * url: 		https://leetcode.com/problems/bitwise-and-of-numbers-range/
 */
int rangeBitwiseAnd(int m, int n) {
	int ret_value = 0;

	// iterate through all unsigned integers with only one bit set
	for (uint32_t single_bit_value = 1; single_bit_value <= (1 << 30); single_bit_value <<= 1) {
		// test two conditions before adding single_bit_value to ret_value
		// 	 1) single_bit_value occurs the same amount of times in both m and n (ie. it
		// 		isn't flipped at all between m and n)
		// 	 2) the bit represented by single_bit_value is on (for m and therefore n as well)
		if ((m / single_bit_value == n / single_bit_value) && ((m & single_bit_value) != 0)) {
			ret_value += single_bit_value;
		}
	}

	return ret_value;
}

/* 268. Missing Number
 *
 * Description:	Find the missing number in a list with n elements containing all but one of the
 * 				numbers 0...n.
 * url: 		https://leetcode.com/problems/missing-number/
 */
int missingNumber(vector<int> &nums) {
	vector<bool> nums_present(nums.size() + 1);	// records the existence of numbers the input vector

	// set the places of all numbers to true in nums_present
	for (auto num : nums) {
		nums_present[num] = true;
	}

	// find and return the missing number
	for (unsigned long i = 0; i < nums_present.size(); i++) {
		if (!nums_present[i]) return i;
	}

	return -1;
}

/* 328. Odd Even Linked List
 *
 * Description: Modify a list so that all elements in odd places within it are brought to the
 * 				beginning of the list with order conserved.
 * url: 		https://leetcode.com/problems/odd-even-linked-list/
 */
ListNode *oddEvenList(ListNode *head) {
	ListNode *lastOdd;		// the element at the end of the current list of odd elements
	ListNode *lastEven;		// the element at the end of the current list of even elements
	ListNode *current;		// the current element being processed
	ListNode *firstEven;	// the first even element that was processed

	// begin by assigning lastOdd, lastEven, and current and ensuring that the list is at least
	// three elements long
	if ((lastOdd = head) == nullptr ||							//first list item
			(lastEven = firstEven = head->next) == nullptr || 	//second list item
			(current = head->next->next) == nullptr) {			//third list item
		//if the list is less than three elements long we don't need to change it
		return head;
	} else {
		// iterate through all remaining list items, adding them to the end of the required list
		for (int i = 3; current != nullptr; i++) {
			if (i % 2 == 0) {
				lastEven->next = current;
				lastEven = current;
				current = current->next;
			} else {
				lastOdd->next = current;
				lastOdd = current;
				current = current->next;
			}
		}

		// add the list of even elements to the end of the list of odd elements
		lastOdd->next = firstEven;
		lastEven->next = nullptr;

		return head;
	}
}

/* 338. Counting Bits
 *
 * Description: Return a count of the number of 1 bits in an integer
 * url: 		https://leetcode.com/problems/counting-bits/
 */
vector<int> countBits(int num) {
	vector<int> return_vector(num + 1);
	int iCopy;

	for (int i = 0; i <= num; i++) {
		return_vector[i] = 0;
		iCopy = i;

		while (iCopy) {
			return_vector[i]++;
			iCopy &= (iCopy - 1);
		}
	}

	return return_vector;
}
