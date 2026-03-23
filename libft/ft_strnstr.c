#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	nlen;
	size_t	i;

	if (!*needle)
		return ((char *)haystack);
	nlen = ft_strlen(needle);
	i = 0;
	while (i + nlen <= len && haystack[i])
	{
		if (ft_strncmp(haystack + i, needle, nlen) == 0)
			return ((char *)(haystack + i));
		i++;
	}
	return (NULL);
}
