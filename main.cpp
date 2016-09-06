#include "medium.cpp"

using namespace std;

ListNode *arrayToLinkedList(int arr[], int arrSize);
void printList(ListNode *list);

int maxSubArray(const vector<int> &A);

int main() {

	return 0;
}

void printList(ListNode *list) {
	while (list != nullptr) {
		cerr << list->val << " ";
		list = list->next;
	}

	cerr << endl;
}

ListNode *arrayToLinkedList(int arr[], int arrSize) {
	ListNode *head = new ListNode(arr[0]);
	ListNode *current = head;

	for (int i = 1; i < arrSize; i++) {
		current->next = new ListNode(arr[i]);
		current = current->next;
	}

	return head;
}


