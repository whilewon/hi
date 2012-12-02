#include <sys/types.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_CAPACITY	(1024 * 1024)

static char *substr(const char *str, unsigned start, 
	unsigned end)
{
	unsigned n = end - start;
	char *sub, *subtemp;
	sub = malloc((n + 1) * sizeof(char));
	strncpy(sub, str + start, n);
	sub[n] = 0;
	return sub;
}
// regex_t *preg;	// a pointer to a pattern buffer storage area;

/* 
 * path: 本地文件路径 
 */
void get_tag_page(char *path)
{
	/* 
	 * <a href="http://hi.baidu.com/hueidou163/item/2713d25bccbb60cdd3e10ca4" class="blog-item blog-text" id="17268399" target="_blank" data-timestamp="1316612396"><div class="text-container">忽然间，想要去很远</div></a> 
	 * /<a href="http:\/\/hi.baidu.com\/[^>]*blog-item blog-text/
	 */
	FILE *fp;
	regex_t preg;
	char *line = malloc(LINE_CAPACITY);
	regmatch_t pmatch;

	const char *pattern = "<a href=\"http://hi.baidu.com/[^>]*blog-item blog-text";

	fp = fopen(path, "r");
	if (fp == NULL) { return; }

	/* compile regex */
	if (regcomp(&preg, pattern, 0)) { return; }

	while (fgets(line, LINE_CAPACITY, fp) != NULL)
	{
		while (1)
		{
			if (regexec(&preg, line, 1, &pmatch, 0))
			{
				break;
			}

			printf("%s\n", substr(line, pmatch.rm_so + 16, pmatch.rm_eo - 28));
			//printf("%s\n", substr(line, pmatch.rm_so, pmatch.rm_eo));

			line += pmatch.rm_eo;
		}
	}



	fclose(fp);
	regfree(&preg);
}
