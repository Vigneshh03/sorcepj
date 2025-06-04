#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 1000
#define MAX_MEMBERS 500

typedef struct {
    int bookID;
    char title[100];
    char author[50];
    int year;
    int totalCopies;
    int copiesAvailable;
} Book;

typedef struct {
    int memberID;
    char name[50];
    char phone[15];
} Member;

typedef struct {
    int issueID;
    int bookID;
    int memberID;
    char issueDate[12];
    char returnDate[12];
    int returned; // 0 = no, 1 = yes
} IssueRecord;

Book books[MAX_BOOKS];
int bookCount = 0;

Member members[MAX_MEMBERS];
int memberCount = 0;

IssueRecord issues[1000];
int issueCount = 0;

// Function declarations
void loadBooks();
void saveBooks();
void loadMembers();
void saveMembers();
void loadIssues();
void saveIssues();

void addBook();
void viewBooks();
void searchBook();
void updateBook();
void deleteBook();

void addMember();
void viewMembers();
void searchMember();

void issueBook();
void returnBook();
void viewIssues();

void mainMenu();
void bookMenu();
void memberMenu();
void issueMenu();

void clearInputBuffer();
void pressEnterToContinue();

int main() {
    loadBooks();
    loadMembers();
    loadIssues();

    mainMenu();

    saveBooks();
    saveMembers();
    saveIssues();

    printf("Exiting program...\n");
    return 0;
}

// Clear input buffer to avoid fgets or scanf issues
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pressEnterToContinue() {
    printf("\nPress Enter to continue...");
    getchar();
}

// Load books from file
void loadBooks() {
    FILE *fp = fopen("books.dat", "rb");
    if (fp == NULL) {
        // No file yet, first run
        return;
    }
    fread(&bookCount, sizeof(int), 1, fp);
    fread(books, sizeof(Book), bookCount, fp);
    fclose(fp);
}

// Save books to file
void saveBooks() {
    FILE *fp = fopen("books.dat", "wb");
    if (fp == NULL) {
        printf("Error saving books data.\n");
        return;
    }
    fwrite(&bookCount, sizeof(int), 1, fp);
    fwrite(books, sizeof(Book), bookCount, fp);
    fclose(fp);
}

// Load members from file
void loadMembers() {
    FILE *fp = fopen("members.dat", "rb");
    if (fp == NULL) {
        return;
    }
    fread(&memberCount, sizeof(int), 1, fp);
    fread(members, sizeof(Member), memberCount, fp);
    fclose(fp);
}

// Save members to file
void saveMembers() {
    FILE *fp = fopen("members.dat", "wb");
    if (fp == NULL) {
        printf("Error saving members data.\n");
        return;
    }
    fwrite(&memberCount, sizeof(int), 1, fp);
    fwrite(members, sizeof(Member), memberCount, fp);
    fclose(fp);
}

// Load issues from file
void loadIssues() {
    FILE *fp = fopen("issues.dat", "rb");
    if (fp == NULL) {
        return;
    }
    fread(&issueCount, sizeof(int), 1, fp);
    fread(issues, sizeof(IssueRecord), issueCount, fp);
    fclose(fp);
}

// Save issues to file
void saveIssues() {
    FILE *fp = fopen("issues.dat", "wb");
    if (fp == NULL) {
        printf("Error saving issues data.\n");
        return;
    }
    fwrite(&issueCount, sizeof(int), 1, fp);
    fwrite(issues, sizeof(IssueRecord), issueCount, fp);
    fclose(fp);
}

// Add new book
void addBook() {
    if (bookCount >= MAX_BOOKS) {
        printf("Book limit reached. Cannot add more.\n");
        return;
    }
    Book b;
    printf("Enter Book ID (integer): ");
    scanf("%d", &b.bookID);
    clearInputBuffer();

    // Check if bookID already exists
    for (int i = 0; i < bookCount; i++) {
        if (books[i].bookID == b.bookID) {
            printf("Book ID already exists. Try again.\n");
            return;
        }
    }

    printf("Enter Book Title: ");
    fgets(b.title, sizeof(b.title), stdin);
    b.title[strcspn(b.title, "\n")] = 0; // remove newline

    printf("Enter Author Name: ");
    fgets(b.author, sizeof(b.author), stdin);
    b.author[strcspn(b.author, "\n")] = 0;

    printf("Enter Year of Publication: ");
    scanf("%d", &b.year);

    printf("Enter Total Copies: ");
    scanf("%d", &b.totalCopies);

    b.copiesAvailable = b.totalCopies;

    books[bookCount++] = b;
    printf("Book added successfully.\n");
}

// View all books
void viewBooks() {
    if (bookCount == 0) {
        printf("No books available.\n");
        return;
    }
    printf("\n%-10s %-30s %-20s %-6s %-8s %-10s\n", "Book ID", "Title", "Author", "Year", "Total", "Available");
    printf("---------------------------------------------------------------------------------------\n");
    for (int i = 0; i < bookCount; i++) {
        printf("%-10d %-30s %-20s %-6d %-8d %-10d\n",
            books[i].bookID, books[i].title, books[i].author,
            books[i].year, books[i].totalCopies, books[i].copiesAvailable);
    }
}

// Search book by title or author
void searchBook() {
    if (bookCount == 0) {
        printf("No books available.\n");
        return;
    }
    char keyword[100];
    printf("Enter keyword to search (title or author): ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = 0;

    int found = 0;
    printf("\nSearch Results:\n");
    printf("%-10s %-30s %-20s %-6s %-8s %-10s\n", "Book ID", "Title", "Author", "Year", "Total", "Available");
    printf("---------------------------------------------------------------------------------------\n");
    for (int i = 0; i < bookCount; i++) {
        if (strstr(books[i].title, keyword) != NULL || strstr(books[i].author, keyword) != NULL) {
            printf("%-10d %-30s %-20s %-6d %-8d %-10d\n",
                books[i].bookID, books[i].title, books[i].author,
                books[i].year, books[i].totalCopies, books[i].copiesAvailable);
            found++;
        }
    }
    if (!found) {
        printf("No matching books found.\n");
    }
}

// Update book info
void updateBook() {
    if (bookCount == 0) {
        printf("No books available to update.\n");
        return;
    }
    int id;
    printf("Enter Book ID to update: ");
    scanf("%d", &id);
    clearInputBuffer();

    int idx = -1;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].bookID == id) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        printf("Book not found.\n");
        return;
    }

    printf("Updating book '%s'\n", books[idx].title);

    printf("Enter new Title (or press Enter to keep current): ");
    char newTitle[100];
    fgets(newTitle, sizeof(newTitle), stdin);
    if (newTitle[0] != '\n') {
        newTitle[strcspn(newTitle, "\n")] = 0;
        strcpy(books[idx].title, newTitle);
    }

    printf("Enter new Author (or press Enter to keep current): ");
    char newAuthor[50];
    fgets(newAuthor, sizeof(newAuthor), stdin);
    if (newAuthor[0] != '\n') {
        newAuthor[strcspn(newAuthor, "\n")] = 0;
        strcpy(books[idx].author, newAuthor);
    }

    printf("Enter new Year (or 0 to keep current): ");
    int newYear;
    scanf("%d", &newYear);
    if (newYear != 0) {
        books[idx].year = newYear;
    }

    printf("Enter new Total Copies (or 0 to keep current): ");
    int newCopies;
    scanf("%d", &newCopies);
    if (newCopies != 0) {
        int diff = newCopies - books[idx].totalCopies;
        books[idx].totalCopies = newCopies;
        books[idx].copiesAvailable += diff;
        if (books[idx].copiesAvailable < 0) {
            books[idx].copiesAvailable = 0;
        }
    }
    clearInputBuffer();

    printf("Book updated successfully.\n");
}

