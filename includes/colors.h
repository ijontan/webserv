/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itan <itan@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 16:32:40 by itan              #+#    #+#             */
/*   Updated: 2024/01/25 16:32:54 by itan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_H
# define COLORS_H

/* --------------------------------- Regular -------------------------------- */
# define BLACK "\033[0;30m"
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define MAGENTA "\033[0;35m"
# define CYAN "\033[0;36m"
# define WHITE "\033[0;37m"

/* --------------------------------- Bolded --------------------------------- */
# define BBLACK "\033[1;30m"
# define BRED "\033[1;31m"
# define BGREEN "\033[1;32m"
# define BYELLOW "\033[1;33m"
# define BBLUE "\033[1;34m"
# define BMAGEN "\033[1;35m"
# define BCYAN "\033[1;36m"
# define BWHITE "\033[1;37m"

/* ------------------------------- Underlined ------------------------------- */
# define UBLACK "\033[4;30m"
# define URED "\033[4;31m"
# define UGREEN "\033[4;32m"
# define UYELLOW "\033[4;33m"
# define UBLUE "\033[4;34m"
# define UMAGEN "\033[4;35m"
# define UCYAN "\033[4;36m"
# define UWHITE "\033[4;37m"

/* ------------------------------- Background ------------------------------- */
# define BLACKB "\033[40m"
# define REDB "\033[41m"
# define GREENB "\033[42m"
# define YELLOWB "\033[43m"
# define BLUEB "\033[44m"
# define MAGENTB "\033[45m"
# define CYANB "\033[46m"
# define WHITEB "\033[47m"

/* ------------------------ High Intensity Background ----------------------- */
# define BLACKHB "\033[0;100m"
# define REDHB "\033[0;101m"
# define GREENHB "\033[0;102m"
# define YELLOWHB "\033[0;103m"
# define BLUEHB "\033[0;104m"
# define MAGENTAHB "\033[0;105m"
# define CYANHB "\033[0;106m"
# define WHITEHB "\033[0;107m"

/* --------------------------- High Intensity Text -------------------------- */
# define HBLACK "\033[0;90m"
# define HRED "\033[0;91m"
# define HGREEN "\033[0;92m"
# define HYELLOW "\033[0;93m"
# define HBLUE "\033[0;94m"
# define HMAGEN "\033[0;95m"
# define HCYAN "\033[0;96m"
# define HWHITE "\033[0;97m"

/* ----------------------- Bolded High Intensity Text ----------------------- */
# define BHBLACK "\033[1;90m"
# define BHRED "\033[1;91m"
# define BHGREEN "\033[1;92m"
# define BHYELL "\033[1;93m"
# define BHBLUE "\033[1;94m"
# define BHMAGEN "\033[1;95m"
# define BHCYAN "\033[1;96m"
# define BHWHITE "\033[1;97m"

/* ---------------------------------- Misc ---------------------------------- */
# define RESET "\033[0m"
# define CLR_TERM "\033[H\033[2J"

#endif