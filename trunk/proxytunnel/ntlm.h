

void build_type1();
int parse_type2(unsigned char *buf);
void build_type3_response();

void build_ntlm2_response();


extern int ntlm_challenge;

char ntlm_type1_buf[160];
char ntlm_type3_buf[4096];


// Below are the flag definitions.
#define NEG_UNICODE		0x00000001
#define NEG_OEM			0x00000002
#define REQ_TARGET		0x00000004
#define NEG_NTLM		0x00000200
#define NEG_DOMAIN		0x00001000
#define NEG_WORK		0x00002000
#define NEG_LOCAL		0x00004000
#define NEG_ASIGN		0x00008000
#define TAR_DOMAIN		0x00010000
#define TAR_SERVER		0x00020000
#define TAR_SHARE		0x00040000
#define NEG_NTLM2		0x00080000
#define NEG_TARINFO		0x00800000
#define	IE_SETSTHIS		0x02000000
#define NEG_128			0x20000000
#define NEG_56			0x80000000

// Below are the NTLM Message Types
#define NTLM_TYPE_1		0x00000001
#define NTLM_TYPE_2		0x00000002
#define NTLM_TYPE_3		0x00000003


typedef struct {
	unsigned short	length;
	unsigned short	space;
	unsigned long	offset;
} security_buf_t;

typedef struct {
	unsigned char	signature[8];
	unsigned long	message_type;
	unsigned long	flags;
	security_buf_t	domain;
	security_buf_t	workstation;
} ntlm_type1;

typedef struct {
	unsigned char	signature[8];
	unsigned long	message_type;
	security_buf_t	target_name;
	unsigned long	flags;
	unsigned char	challenge[8];
	unsigned long	context1;
	unsigned long	context2;
	security_buf_t	target_info;
	unsigned char	data_start;
} ntlm_type2;

typedef struct {
	unsigned char	signature[8];
	unsigned long	message_type;
	security_buf_t	LM_response;
	security_buf_t	NTLM_response;
	security_buf_t	domain;
	security_buf_t	user;
	security_buf_t	workstation;
	unsigned char	session[8];
	unsigned long	flags;
	unsigned char	pad[8];

} ntlm_type3;


typedef struct {
	unsigned char	digest[8];
	unsigned char	challenge[8];
	unsigned long	signature;
	unsigned long	reserved;
	unsigned long long	timestamp;
	unsigned char	client_challenge[8];
	unsigned long	unknown;
	unsigned long	data_start;
} blob;

