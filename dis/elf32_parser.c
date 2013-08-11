#include <elf.h>
typedef struct{
    long size;
    long offset;
}elf_data_t;

int ELF32_GetStrTable(char *fname, char **output,elf_data_t *text)
{
    Elf32_Ehdr *hdr;
    Elf32_Shdr *shdr;
    char *strtable;
    char *strings;
    void *buff;
    qboolean textfound = qfalse;
    qboolean dynstrfound = qfalse;
    int j,nstrings = 0;
    long len;
    FILE *fp = fopen(fname,"rb");
    if(!fp){
	return qfalse;
    }
    fseek(fp,0,SEEK_END);
    len = ftell(fp);
    rewind(fp);
    if(len<=0)
	return qfalse;
    buff = malloc(len);
    
    if(buff == NULL)
	return qfalse;
    if(fread(buff,len,1,fp)!=1){
	if(buff)
	    free(buff);
	return qfalse;
    }
    hdr = buff;
    //Com_Printf("Debug1\n");
    /*if(fread(&hdr,sizeof(Elf32_Ehdr),1,fp)!= 1){
	return qfalse;
    }*/
    //memcpy(&hdr,buff,sizeof(Elf32_Ehdr));
    //Com_Printf("Debug1.4\n");
    if(hdr->e_ident[0] != ELFMAG0 || hdr->e_ident[1] != ELFMAG1 || hdr->e_ident[2] != ELFMAG2 || hdr->e_ident[3] != ELFMAG3){
	if(buff)
	    free(buff);
	return qfalse;
    
    }
    if(hdr->e_type != ET_DYN){
	if(buff)
	    free(buff);
	return qfalse;
    }
    //Com_Printf("Debug1.5\n");
    //fseek(fp,hdr.e_shoff,SEEK_SET);
    if((shdr = (Elf32_Shdr *)(buff + hdr->e_shoff))>=(Elf32_Shdr *)(buff + len-sizeof(Elf32_Shdr))){
	if(buff)
	    free(buff);
	return qfalse;
    }
    //if(fread(shdr,hdr.e_shentsize,hdr.e_shnum,fp)!=hdr.e_shnum){
//	return qfalse;
//    }
    //Com_Printf("Debug1.6\n");
    if(hdr->e_shstrndx!=SHN_UNDEF){
	if(hdr->e_shstrndx!=SHN_XINDEX){ 	// Typical case, standard elf addressing
	    if(hdr->e_shstrndx<=hdr->e_shnum)
		strtable = buff + shdr[hdr->e_shstrndx].sh_offset;
	    else{
		Com_Printf("Error: the string table index is too big! String table index: %d, section headers: %d.\n",hdr->e_shstrndx,hdr->e_shnum);
		if(buff)
		    free(buff);
		return qfalse;
	    }
	}else{		// So called 'elf extended addressing'. Because 'simple' is too mainstream.
	    if(shdr[0].sh_link<=hdr->e_shnum)
		strtable = buff + shdr[shdr[0].sh_link].sh_offset;
	    else{
		Com_Printf("Error: the string table index is too big! String table index: %d, section headers: %d.\n",shdr[0].sh_link,hdr->e_shnum);
		if(buff)
		    free(buff);
		return qfalse;
	    }
	}
    }
    else{
	Com_Printf("Could not find the string table.\n");
	if(buff)
	    free(buff);
	return qfalse;
    }
    //Com_Printf("Debug2\n");
    for(j=0;j<hdr->e_shnum;++j){
	if(strcmp(&strtable[shdr[j].sh_name],".text")==0){
	    textfound = qtrue;
	    text->size = shdr[j].sh_size;
	    text->offset = shdr[j].sh_addr;
	    //Com_Printf("Debug: .text section found! Size: %d, address: %d.\n",shdr[j].sh_size,shdr[j].sh_addr);
	    if(dynstrfound)
		break;
	}
	else if(strcmp(&strtable[shdr[j].sh_name],".dynstr")==0){
	    dynstrfound = qtrue;
	    nstrings = shdr[j].sh_size;
	    strings = (char *)malloc(sizeof(char)*nstrings);
	    strings = malloc(shdr[j].sh_size);
	    memcpy(strings,shdr[j].sh_offset + buff,shdr[j].sh_size);
	    *output = strings;
	    if(textfound)
		break;
	}
    }
    if(textfound && dynstrfound){
	if(buff)
	    free(buff);
	return nstrings;
	
    }else{
	if(buff)
	    free(buff);
	return qfalse;
    }
}