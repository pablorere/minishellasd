#include "libft.h"

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	size_t	slen;
	char	*result;
	size_t	actual_len;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	if (start >= slen)
		return (ft_strdup(""));
	actual_len = slen - start;
	if (actual_len > len)
		actual_len = len;
	result = malloc(actual_len + 1);
	if (!result)
		return (NULL);
	ft_memcpy(result, s + start, actual_len);
	result[actual_len] = '\0';
	return (result);
}
