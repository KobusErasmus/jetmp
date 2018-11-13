#define MAX_KEYS 100
#define MAX_KEY_LENGTH 51
#define MAX_VALUE_LENGTH 1001

char keys_array[MAX_KEYS][MAX_KEY_LENGTH];
char values_array[MAX_KEYS][MAX_VALUE_LENGTH];
_Bool escape_html = 0;
_Bool searching_for_key = 1;
_Bool searching_for_value = 0;
_Bool searching_for_colon = 0;
_Bool adding_key = 0;
_Bool adding_value = 0;
_Bool is_value_a_string = 0;
_Bool previous_ch_escape = 0;
char word[MAX_VALUE_LENGTH];
char character;
int word_index = 0;
int value_index = 0;

void json_to_array(char json[]);
char* find_json_value(char key[]);
void add_char_to_word(char c);
static void add_char_to_value(char c);
static void add_string_to_word(char[]);
void end_adding_key(void);
void end_adding_value(void);
void search_for_key(void);
void add_key_char(void);
void search_for_colon(void);
void search_for_value(void);
void add_value_char(void);
void analyse_character(void);
_Bool reached_end_of_word(void);
