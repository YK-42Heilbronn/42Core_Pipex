/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 09:37:27 by ykonka            #+#    #+#             */
/*   Updated: 2026/02/09 13:28:20 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;

	while (*lst != NULL)
	{
		temp = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = temp;
	}
}

// void int_del(void *content) {
//     (void)content;  // Do nothing - not malloc'd
// }

// #include "stdint.h"
// #include "stdio.h"

// int main(){
// 	t_list *head;
// 	t_list *links;
// 	int i;
// 	// int j;
// 	// int k;
// 	// int l;

// 	i = 21;
// 	head = ft_lstnew((void*)(intptr_t)i);
// 	i = 20;
// 	links = ft_lstnew((void*)(intptr_t)i);
// 	ft_lstadd_back(&head, links);
// 	i = 19;
// 	links = ft_lstnew((void*)(intptr_t)i);
// 	ft_lstadd_back(&head, links);
// 	i = 18;
// 	links = ft_lstnew((void*)(intptr_t)i);
// 	ft_lstadd_back(&head, links);
// 	// printf("%d ", (int)(intptr_t)head->content);
// 	t_list *next;
// 	next = head;
// 	while(next){
// 		printf("%d ", (int)(intptr_t)next->content);
// 		next = next->next;
// 	}
// 	ft_lstclear(&head, int_del);
// 	next = head;
// 	if (next==NULL)
// 		printf("NULL\n");
// 	else
// 		printf("not NULL\n");
// 	while(next){
// 		printf("%d ", (int)(intptr_t)next->content);
// 		next = next->next;
// 	}
// }