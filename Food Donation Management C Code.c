#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Food {
    int id, quantity, hoursLeft;
    char name[50], source[50];
};
struct NGO {
    int id;
    char name[50], location[50];
    struct NGO *next;
};
struct Donation {
    char food[50], ngo[50];
    int quantity;
    struct Donation *next;
};
struct Food heap[100];
int size = 0, foodId = 1, ngoId = 1;

struct NGO *ngoHead = NULL;
struct Donation *donationHead = NULL;

void swap(struct Food *a, struct Food *b) {
    struct Food t = *a;
    *a = *b;
    *b = t;
}
/* Min-Heap */
void heapifyUp(int i) {
    while (i > 0) {
        int p = (i - 1) / 2;
        if (heap[p].hoursLeft <= heap[i].hoursLeft)
            break;
        swap(&heap[p], &heap[i]);
        i = p;
    }
}

void heapifyDown(int i) {
    int s, l, r;
    while (1) {
        s = i;
        l = 2 * i + 1;
        r = 2 * i + 2;

        if (l < size && heap[l].hoursLeft < heap[s].hoursLeft)
            s = l;
        if (r < size && heap[r].hoursLeft < heap[s].hoursLeft)
            s = r;

        if (s == i) break;
        swap(&heap[i], &heap[s]);
        i = s;
    }
}
void insertFood(struct Food f) {
    if (size == 100) {
        printf("Storage full!\n");
        return;
    }
    heap[size] = f;
    heapifyUp(size);
    size++;
}
struct Food removeFood() {
    struct Food f = heap[0];
    heap[0] = heap[size - 1];
    size--;
    if (size > 0) heapifyDown(0);
    return f;
}
void addFood() {
    struct Food f;
    f.id = foodId++;

    printf("Food name: ");
    scanf(" %49[^\n]", f.name);
    printf("Source: ");
    scanf(" %49[^\n]", f.source);
    printf("Quantity: ");
    scanf("%d", &f.quantity);
    printf("Hours left: ");
    scanf("%d", &f.hoursLeft);

    if (f.quantity <= 0 || f.hoursLeft < 0) {
        printf("Invalid details!\n");
        return;
    }

    insertFood(f);
    printf("Food added! ID: %d\n", f.id);
}

void displayFood() {
    if (size == 0) {
        printf("No food available.\n");
        return;
    }

    for (int i = 0; i < size; i++)
        printf("\nID: %d | %s | %s | %d meals | %d hours\n",
               heap[i].id, heap[i].name, heap[i].source,
               heap[i].quantity, heap[i].hoursLeft);
}

void searchFood() {
    int id;
    printf("Enter Food ID: ");
    scanf("%d", &id);

    for (int i = 0; i < size; i++) {
        if (heap[i].id == id) {
            printf("%s | %s | %d meals | %d hours\n",
                   heap[i].name, heap[i].source,
                   heap[i].quantity, heap[i].hoursLeft);
            return;
        }
    }
    printf("Food not found.\n");
}

void registerNGO() {
    struct NGO *n = malloc(sizeof(struct NGO));
    if (n == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    n->id = ngoId++;
    printf("NGO name: ");
    scanf(" %49[^\n]", n->name);
    printf("Location: ");
    scanf(" %49[^\n]", n->location);
    n->next = NULL;

    if (ngoHead == NULL)
        ngoHead = n;
    else {
        struct NGO *t = ngoHead;
        while (t->next != NULL) t = t->next;
        t->next = n;
    }

    printf("NGO registered! ID: %d\n", n->id);
}

void displayNGOs() {
    struct NGO *t = ngoHead;
    while (t != NULL) {
        printf("\nID: %d | %s | %s\n", t->id, t->name, t->location);
        t = t->next;
    }
    if (ngoHead == NULL) printf("No NGOs registered.\n");
}

void saveDonation(char food[], char ngo[], int qty) {
    struct Donation *d = malloc(sizeof(struct Donation));
    if (d == NULL) return;

    strcpy(d->food, food);
    strcpy(d->ngo, ngo);
    d->quantity = qty;
    d->next = NULL;

    if (donationHead == NULL)
        donationHead = d;
    else {
        struct Donation *t = donationHead;
        while (t->next != NULL) t = t->next;
        t->next = d;
    }
}
void allocateDonation() {
    int id, req;
    struct NGO *n = ngoHead;

    printf("Enter NGO ID: ");
    scanf("%d", &id);

    while (n != NULL && n->id != id) n = n->next;

    if (n == NULL) {
        printf("NGO not found.\n");
        return;
    }
    printf("Required meals: ");
    scanf("%d", &req);

    if (req <= 0 || size == 0) {
        printf("Invalid quantity or no food available.\n");
        return;
    }
    while (req > 0 && size > 0) {
        struct Food *f = &heap[0];
        int qty = (f->quantity < req) ? f->quantity : req;

        printf("Donating %d meals of %s\n", qty, f->name);
        saveDonation(f->name, n->name, qty);

        f->quantity -= qty;
        req -= qty;

        if (f->quantity == 0)
            removeFood();
    }

    if (req == 0)
        printf("Donation completed for %s!\n", n->name);
    else
        printf("Food shortage! Remaining: %d meals\n", req);
}
void donationHistory() {
    struct Donation *t = donationHead;
    while (t != NULL) {
        printf("\nFood: %s | NGO: %s | Meals: %d\n",
               t->food, t->ngo, t->quantity);
        t = t->next;
    }
    if (donationHead == NULL) printf("No donation history.\n");
}
void totalMeals() {
    int total = 0;
    for (int i = 0; i < size; i++)
        total += heap[i].quantity;
    printf("Total available meals: %d\n", total);
}
int main() {
    int ch;
    do {
        printf("\n FOOD RESCUE SYSTEM \n");
        printf("1. Add Food\n2. Display Food\n3. Search Food\n");
        printf("4. Register NGO\n5. Display NGOs\n6. Allocate Donation\n");
        printf("7. Donation History\n8. Total Meals\n");
        printf("9. Highest Priority Food\n10. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &ch);
        switch (ch) {
            case 1: addFood(); break;
            case 2: displayFood(); break;
            case 3: searchFood(); break;
            case 4: registerNGO(); break;
            case 5: displayNGOs(); break;
            case 6: allocateDonation(); break;
            case 7: donationHistory(); break;
            case 8: totalMeals(); break;
            case 9:
                if (size > 0)
                    printf("%s | %d hours left | %d meals\n",
                           heap[0].name, heap[0].hoursLeft,
                           heap[0].quantity);
                else
                    printf("No food available.\n");
                break;
            case 10: printf("Thank you!\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (ch != 10);
    return 0;
}
