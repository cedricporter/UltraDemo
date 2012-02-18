#include "stdafx.h"
#include "syntax.h"

CString g_strInString; //定义一个字符串值除了包含字符数字、下划线_外，还包含什么字符常量, C++还会包含#, 如#include
TCHAR *g_BeginCommentExStr[2];//当前语言的多行注释规则：引起多行注释的字符串
TCHAR *g_EndCommentExStr[2]; //当前语言的多行注释规则：结束多行注释的字符串
TCHAR *g_LineCommentStr[2]; //当前语言的单行注释规则, 以行末为中止
TCHAR *g_BeginStringStr[1]; //当前语言的字符规则,　引起字符串变量的字符串
TCHAR *g_EndStringStr[1]; //当前语言的字符规则,　结束字符串变量的字符串
TCHAR *g_BeginCharStr[1]; //当前语言的字符规则,　结束字符型常量的字符串
TCHAR *g_EndCharStr[1]; //当前语言的字符规则,　结束字符型常量的字符串
BOOL g_bControl; //控制字符，如C++中的\"，因为控制字符\的存在其表示的是一个引号
BOOL g_bCase; //在进行关键字比较时是否区分大小写

int g_nstrInStringCount; 
int g_nBeginCommentExStrCount; 
int g_nEndCommentExStrCount; 
int g_nLineCommentStrCount; 
int g_nBeginStringStrCount; 
int g_nEndStringStrCount; 
int g_nBeginCharStrCount; 
int g_nEndCharStrCount; 

int g_nCurLanguage;

void SetCurLanguage(int nLanguage)
{
	g_nCurLanguage = nLanguage;
}
// 是不是关键字
BOOL IsSynWord(CStringArray &m_strArrayKeyWords, CString &strReadyToTest) 
{
	int val;
	int low=0,hig=m_strArrayKeyWords.GetSize()-1;
	int mid=(low+hig)/2;
 // 二分查找关键字
	while(low<=hig) {
		CString str = m_strArrayKeyWords[mid];
		if(g_bCase)
			val=strReadyToTest.Compare(m_strArrayKeyWords[mid]);
		else
			val=strReadyToTest.CompareNoCase(m_strArrayKeyWords[mid]);
		if(val>0)
			low=mid+1;
		else if(val<0)
			hig=mid-1;
		else
			return TRUE;
		mid=(low+hig)/2;
	}
	return FALSE;
}

// 是不是数字
BOOL IsNumber(CString &strReadyToTest)
{
	int nLength = strReadyToTest.GetLength(); 
	//*
	switch (g_nCurLanguage)
	{
		case _CPP:
			{ // 判断十六进制
				if (nLength > 2 && strReadyToTest[0] == _T('0') && 
					strReadyToTest[1] == _T('x'))
				{
					for (int I = 2; I < nLength; I++)
					{
						if (_istdigit(strReadyToTest[I])  || (strReadyToTest[I] >= _T('A') && 
							strReadyToTest[I] <= _T('F')) ||(strReadyToTest[I] >= _T('a') && 
							strReadyToTest[I] <= _T('f')))
							continue;
						return FALSE;
					}
					return TRUE;
				}
			}
			break;
		case _OBJECTPASCAL:
			{
				if (strReadyToTest[0] == _T('$'))
				{
					for (int I = 1; I < nLength; I ++)
					{
						if (_istdigit(strReadyToTest[I]) ||(strReadyToTest[I] >= _T('A') && 
							strReadyToTest[I] <= _T('F')) ||
							(strReadyToTest[I] >= _T('a') && strReadyToTest[I] <= _T('f')))
							continue;
						return FALSE;
					}
					return TRUE;
				}		
			}
			break;
		case _BASIC:
			{
				if (nLength > 2 && strReadyToTest[0] == _T('&') && 
					(strReadyToTest[1] == _T('O') || strReadyToTest[1] == _T('H')))
				{
					for (int I = 2; I < nLength; I++)
					{
						if (_istdigit(strReadyToTest[I])  ||(strReadyToTest[I] >= _T('A') && 
							strReadyToTest[I] <= _T('F')) ||(strReadyToTest[I] >= _T('a') && 
							strReadyToTest[I] <= _T('f')))
							continue;
						return FALSE;
					}
					return TRUE;
				}
			}
			break;
		case _HTML:
			{
				if (nLength > 2 && strReadyToTest[0] == '#')
				{
					for (int I = 1; I < nLength; I++)
					{
						if (_istdigit(strReadyToTest[I]) || (strReadyToTest[I] >= 'A' && 
							strReadyToTest[I] <= 'F') ||
							(strReadyToTest[I] >= 'a' && strReadyToTest[I] <= 'f'))
							continue;
						return FALSE;
					}
					return TRUE;
				}		
			}
			break;

		case NONE:
			return FALSE;

		default:
			break;
	} // switch
	
	if (!_istdigit(strReadyToTest[0]))
		return FALSE;
	
	// 判断普通数字
	for (int I = 1; I < nLength; I++)
	{
		if (!_istdigit(strReadyToTest[I]) && strReadyToTest[I] != _T('+') &&
			strReadyToTest[I] != _T('-')  && strReadyToTest[I] != _T('.') && 
			strReadyToTest[I] != _T('e')  && strReadyToTest[I] != _T('E'))
			return FALSE;
	}
	//*/
	return TRUE;
}


//装入关键字列表及语法解析规则
void LoadSynWord(CStringArray &m_strArrayKeyWords, int nLanguage)
{
	LoadParseRule(nLanguage);

	CString strAllSynWord;
	m_strArrayKeyWords.RemoveAll();
	
	CWinApp *app = AfxGetApp();
	switch ( nLanguage ) {
	case _ADA:
		strAllSynWord = _T(" abort,abs,abstract,accept,access,aliased,all,and,at,begin,body,case,declare,delay,delta,digits,do,else,elsif,end,entry,exception,exit,for,function,generic,goto,if,in,is,loop,mod,new,not,null,of,or,others,out,package,pragma,procedure,protected,raise,rem,renames,requeue,return,reverse,select,separate,task,terminate,then,until,use,when,while,with,xor,array,constant,limited,record,type,subtype,range,private,tagged,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_ADA"), strAllSynWord);
		break;
	case _ASM:
		strAllSynWord = _T(" eax,ebx,ecx,edx,ebp,esp,esi,edi,AAA,AAD,AAM,AAS,ADC,ADD,AND,ARPL,BOUND,BSF,BSR,BSWAP,BT,BTC,BTR,BTS,CALL,CBW,CDQ,CLC,CLD,CLI,CLTS,CMC,CMOVA,CMOVAE,CMOVB,COMVBE,CMOVC,CMOVE,CMOVG,CMOVGE,CMOVL,CMOVLE,CMOVNA,CMOVNAE,CMOVNB,CMOVNBE,CMOVNC,CMOVNE,CMOVNG,CMOVNGE,CMOVNL,CMOVNLE,CMOVNO,CMOVNP,CMOVNS,CMOVNZ,CMOVO,CMOVP,CMOVPE,CMOVPO,CMOVS,CMOVZ,CMP,CMPS,CMPSB,CMPSW,CMPSD,CMPXCHG,CMPXCHG8B,CPUID,CWD,CWDE,DAA,DAS,DEC,DIV,ENTER,HLT,IDIV,IMUL,IN,INC,INS,INT,INTO,INVD,INVLPG,IRET,IRETD,JA,JAE,JB,JBE,JC,JCXZ,JECXZ,JE,JG,JGE,JL,JLE,JNA,JNAE,JNB,JNBE,JNC,JNE,JNG,JNGE,JNL,JNLE,JNO,JNP,JNS,JNZ,JO,JP,JPE,JPO,JS,JZ,JMP,LAHF,LAR,LDS,LEA,LEAVE,LES,LFS,LGDT,LGS,LIDT,LLDT,LMSW,LOCK,LODS,LODSB,LODSW,LODSD,LOOP,LOOPZ,LOOPE,LOOPNZ,LOOPNE,LSL,LSS,LTR,MOV,MOVS,MOVSB,MOVSW,MOVSD,MOVSX,MOVZX,MUL,NEG,NOP,NOT,OR,OUT,OUTS,POP,POPA,POPAD,POPF,POPFD,PUSH,PUSHA,PUSHAD,PUSHF,PUSHFD,RCL,RCR,RDMSR,RDPMC,RDTSC,REP,REPE,REPNE,RET,ROL,ROR,RSM,SAHF,SAL,SAR,SBB,SCAS,SCASB,SCASW,SCASD,SETA,SETAE,SETB,SETBE,SETC,SETE,SETG,SETGE,SETL,SETLE,SETNA,SETNAE,SETNB,SETNBE,SETNC,SETNE,SETNG,SETNGE,SETNL,SETNLE,SETNO,SETNP,SETNS,SETNZ,SETO,SETP,SETPE,SETPO,SETS,SETZ,SGDT,SHL,SHLD,SHR,SHRD,SIDT,SLDT,SMSW,STC,STD,STI,STOS,STOSB,STOSW,STOSD,STR,SUB,TEST,UD2,VERR,VERW,WAIT,WBINVD,WRMSR,XADD,XCHG,XLAT,XLATB,XOR,EMMS,MOVD,MOVQ,PACKSSDW,PACKSSWB,PACKUSWB,PADD,PADDS,PADDUS,PAND,PANDN,PCMPEQ,PCMPGT,PMADD,PMULH,PMULL,POR,PSLL,PSRA,PSRL,PSUB,PSUBS,PSUBUS,PUNPCKH,PUNPCKL,PXOR,F2XM1,FABS,FADD,FADDP,FBLD,FBSTP,FCHS,FCLEX,FCMOVB,FCMOVE,FCMOVBE,FCMOVU,FCMOVNB,FCMOVNE,FCMOVNBE,FCOM,FCOMP,FCOMPP,FCOMI,FCOMIP,FCOS,FDECSTP,FDIV,FDIVP,FDIVR,FDIVRP,FFREE,FIADD,FICOM,FICOMP,FIDIV,FIDIVR,FILD,FIMUL,FINCSTP,FINIT,FIST,FISTP,FISUB,FISUBR,FLD,FLD1,FLDCW,FLDENV,FLDL2E,FLDL2T,FLDLG2,FLDLN2,FLDPI,FLDZ,FMUL,FMULP,FNOP,FPATAN,FPREM,FPREM1,FPTAN,FRNDINT,FRSTOR,FSAVE,FSCALE,FSIN,FSINCOS,FSQRT,FST,FSTCW,FSTENV,FSTP,FSTSW,FSUB,FSUBP,FSUBR,FSUBRP,FTST,FUCOM,FUCOMP,FUCOMPP,FUCOMI,FUCOMIP,FXAM,FXCH,FXTRACT,FYL2X,FYL2XP1,FWAIT,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_ASM"), strAllSynWord);
		break;
	case _BASIC:
		strAllSynWord = _T(" Alias,And,Any,As,Base,Boolean,Byref,Byte,Byval,Call,Case,Close,Compare,Const,Currency,Data,Date,Declare,Defbool,Defbyte,Defcur,Defdate,Defdbl,Defdec,Defint,Deflng,Defobj,Defsng,Defstr,Defvar,Dim,Do,Double,Each,Else,Elseif,End,Enum,Eqv,Erase,Error,Event,Exit,Explicit,False,For,Function,Get,Global,Gosub,Goto,If,IIf,Imp,Implements,In,Integer,Is,Let,Lib,Line,Lock,Long,Loop,Lset,New,Next,Not,Object,On,Open,Option,Optional,Or,Preserve,Print,Private,Property,Public,Put,Raiseevent,Redim,Resume,Return,Rset,Select,Set,Single,Static,Stop,String,Sub,Then,To,True,Type,Ubound,Unload,Unlock,Var,Variant,Wend,While,With,Write,Xor,#if,#End,#else,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_BASIC"), strAllSynWord);
		break;
	case _BATCH:
		strAllSynWord = _T(" do,echo,errorlevel,exist,for,goto,if,in,not,off,on,pause,set,shift,assign,attrib,backup,break,cd,chdir,chkdsk,cls,copy,ctty,date,del,dir,diskcomp,diskcopy,erase,exe2bin,fdisk,find,format,graftabl,graphics,join,label,md,mkdir,mode,more,path,print,prompt,recover,ren,rename,replace,restore,rm,rmdir,select,share,sort,substitute,sys,time,tree,type,ver,verify,vol,xcopy,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_BATCH"), strAllSynWord);
		break;
	case _BLUE:
		strAllSynWord = _T(" class,is,uses,internal,const,var,routines,interface,creation,invariant,end,pre,post,enumeration,manifest,deferred,builtin,redefined,return,assert,super,if,then,else,case,of,loop,exit,on,in,create,old,this,input,output,format,termiinal,Integer,Boolean,Real,String,LList,LIndexList,LStack,AStack,LQueue,AQueue,HPrioQueue,LPrioQueue,APrioQueue,HSymbolTable,TSymbolTable,Tree,BinaryTree,Matrix,Array,Set,Bag,TextFileHandle,FileSysHandle,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_BLUE"), strAllSynWord);
		break;
	case _CLIPPER:
		strAllSynWord = _T(" .and.,.f.,.not.,.or.,.t.,accept,all,alternate,append,average,begin,bell,blank,box,break,call,cancel,case,ccept,century,clear,close,color,commit,confirm,console,continue,copy,count,create,cursor,date,decimals,declare,default,define,delimiters,device,dir,display,do,eject,else,elseif,end,endcase,enddo,endif,epoch,erase,escape,exact,exclusive,extended,external,file,filter,find,fixed,for,form,format,from,function,get,go,goto,if,index,input,intensity,join,key,keyboard,label,list,local,locate,loop,margin,memory,memvar,menu,message,new,next,nil,note,on,order,otherwise,pack,path,picture,printer,private,procedure,prompt,public,quit,read,recall,reindex,relation,release,rename,replace,report,restore,return,run,save,say,scoreboard,screen,sequence,set,shared,skip,softseek,sort,static,store,structure,sum,text,to,total,type,typeahead,unique,unlock,update,use,valid,wait,while,wrap,zap,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_CLIPPER"), strAllSynWord);
		break;
	case _COBOL:
		strAllSynWord = _T(" ACCEPT,ACCESS,ADD,ADVANCING,AFTER,ALL,ALPHABETIC,ALPHABETIC-LOWER,ALPHABETIC-UPPER,ALPHANUMERIC,ALSO,AND,ANY,ARE,ASCENDING,ASSIGN,AT,AUTHOR,BEFORE,BEGINNING,BLANK,BY,CALL,CANCEL,CHARACTER,CHARACTERS,CLOSE,COMMA,COMMIT,COMP,COMP-3,COMP-5,COMPUTE,CONFIGURATION,CONTAINS,CONTINUE,CONVERTING,DATA,DECIMAL-POINT,DELETE,DELIMITED,DELIMITER,DEPENDING,DESCENDING,DISPLAY,DIVIDE,DIVISION,DO,DUPLICATES,ELSE,END,END-ACCEPT,END-CALL,END-COMPUTE,END-DELETE,END-EVALUATE,END-IF,END-MULTIPLY,END-PERFORM,END-READ,END-REWRITE,END-SEARCH,END-START,END-STRING,END-SUBTRACT,END-UNSTRING,END-WRITE,ENVIRONMENT,EVALUATE,EXEC CICS,END-EXEC,EXIT,EXTEND,FD,FILE,FILE-CONTROL,FILLER,FROM,GIVING,GO,GOBACK,HIGH-VALUE,HIGH-VALUES,I-O,IDENTIFICATION,IF,IN,INDEXED,INITIALIZE,INTO,INPUT,INPUT-OUTPUT,INSERT,INVALID,IS,LEADING,LENGTH,LINKAGE,LOW-VALUE,LOW-VALUES,MOVE,MULTIPLY,NEGATIVE,NOT,NUMERIC,NUMERIC-EDITED,OCCURS,ON,OPEN,OR,ORGANIZATION,OUTPUT,PAGE,PERFORM,PIC,POSITIVE,PROCEDURE,PROGRAM-ID,RANDOM,READ,REDEFINES,REMAINDER,REPLACE,REPLACING,RETURN,RETURN-CODE,REWRITE,ROUNDED,RUN,SD,SEARCH,SECTION,SELECT,SEPERATE,SET,SIGN,SIZE,SORT,SPACE,SPACES,SPECIAL-NAMES,STATUS,STRING,SUBTRACT,TALLY,TALLYING,THEN,THRU,TO,TRAILING,TRUE,UNSTRING,UNTIL,USAGE,USING,VALUE,VARYING,WHEN,WORKING-STORAGE,WRITE,X,ZERO,ZEROES,ZEROS,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_COBOL"), strAllSynWord);
		break;
	case _CPP:
		strAllSynWord = _T(" #define,#else,#elif,#elseif,#endif,#error,#if,#ifdef,#ifndef,#include,#pragma,#undef,__asm,__based,__cdecl,__declspec,__except,__fastcall,__finally,__inline,__int16,__int32,__int64,__int8,__leave,__multiple_inheritance,__pascal,__single_inheritance,__stdcall,__try,__uuidof,__virtual_inheritance,_asm,_cdecl,_fastcall,_pascal,_stdcall,afx_msg,auto,bool,break,case,catch,char,class,code_seg,const,const_cast,continue,default,defined,delete,dllexport,dllimport,do,double,dynamic_cast,else,enum,explicit,extern,false,float,for,friend,goto,if,inline,int,interface,long,main,mutable,naked,namespace,new,off,on,once,operator,pack,pascal,pop,private,protected,public,push,register,reinterpret_cast,return,short,signed,sizeof,static,static_cast,struct,switch,template,this,thread,throw,true,try,typedef,typeid,typename,union,unsigned,using,uuid,virtual,void,volatile,while,wmain,xalloc,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_CPP"), strAllSynWord);
		break;
	case _CSHAPE:
		strAllSynWord = _T(" #define,#else,#elif,#elseif,#endif,#error,#if,#ifdef,#ifndef,#include,#pragma,#undef,__asm,__based,__cdecl,__declspec,__except,__fastcall,__finally,__inline,__int16,__int32,__int64,__int8,__leave,__multiple_inheritance,__pascal,__single_inheritance,__stdcall,__try,__uuidof,__virtual_inheritance,_asm,_cdecl,_fastcall,_pascal,_stdcall,afx_msg,abstract,as,base,bool,break,byte,case,catch,char,checked,class,const,continue,decimal,default,delegate,do,double,else,enum,event,explicit,extern,false,finally,fixed,float,for,foreach,goto,if,implicit,in,int,interface,internal,is,lock,long,namespace,new,null,object,operator,out,override,params,private,protected,public,readonly,ref,return,sbyte,sealed,short,sizeof,stackalloc,static,string,struct,switch,this,throw,true,try,typeof,uint,ulong,unchecked,unsafe,ushort,using,virtual,void,while,true,false,null,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_CSHAPE"), strAllSynWord);
		break;
	case _DATAFLEX:
		strAllSynWord = _T(" abs,accept,acos,append_output,arc,asin,atan,autopage,backfield,blankform,box,calculate,channel,chart,chart_init,circle,clearform,clearscreen,clearxy,clear_option,close_input,close_output,cmdline,copyfile,cos,decrement,directory,direct_input,direct_output,diskfree,display,display_cut,display_graphic,ellipse,endgroup,entagain,entdisplay,enter,enterend,entergroup,entry,erasefile,exp,flood,format,formfeed,getdrange,get_argument_size,get_channel_position,get_current_colors,get_current_directory,get_current_input_channel,get_current_lockcount,get_current_output_channel,get_current_user_count,get_date_format,get_environment,get_grxy,get_licensed_max_users,get_number_format,get_video_mode,gotoxy,graphic,graph_getfirst,graph_getnext,graph_init,graph_stat,graph_sum,graph_value,grxy,gr_print,help,hi,ifchange,increment,inkey,inkey$,input,is_dse_license,keycheck,keyproc,line,log,low,memory,mod,move,movedate,moveint,movenum,movereal,movestr,output,output_wrap,page,pagecheck,palette,pie,plot,print,print_wrap,random,read,readln,read_block,read_dfini,registration,renamefile,report,reportend,round,runprogram,save_cut,save_graphic,screenmode,section,setchange,setworld,set_argument_size,set_channel_position,set_dfpath,set_line_style,set_option,set_termlist,set_text,set_video_mode,show,showln,sin,sleep,sqrt,subtotal,sysconf,sysdate,tan,write,writeln,date,indicator,integer,local,name,number,real,string,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_COBOL"), strAllSynWord);
		break;
	case _EIFFEL:
		strAllSynWord = _T(" and,as,check,class,Clone,Create,Current,debug,deferred,div,do,else,elsif,end,ensure,Equal,export,external,false,feature,Forget,from,if,inherit,invariant,is,language,like,local,loop,mod,name,nochange,not,old,once,or,redefine,rename,require,rescue,Result,retry,then,until,variant,Void,BOOLEAN,CHARACTER,INTEGER,REAL,STRING,append,char_string,clear,duplicate,enter,entry,fill_blank,gt,hash_code,head,le,left_adjust,length,prepend,remove_all_occurrences,remove_char,resize,right_adjust,same_as,share,shared_with,size,string_char,substring,tail,to_c,to_integer,to_lower,to_upper,close,creatable,date,error,executable,exists,lastchar,lastint,laststring,next_line,open_append,open_read,open_write,putbool,putbool_nl,putchar,putchar_nl,putint,putint_nl,putreal,putreal_nl,putstring,putstring_nl,readable,set_error_output,unset_error_output,writable,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_EIFFEL"), strAllSynWord);
		break;
	case _FORTRAN:
		strAllSynWord = _T(" ALLOCATABLE,CHARACTER,COMMON,COMPLEX,DATA,DIMENSION,DOUBLE,EQUIVALENCE,EXTERNAL,IMPLICIT,IN,INOUT,INTEGER,INTENT,INTRINSIC,KIND,LOGICAL,NAMELIST,NONE,OPTIONAL,OUT,PARAMETER,POINTER,PRIVATE,PUBLIC,REAL,RESULT,SAVE,SEQUENCE,TARGET,TYPE,USE,ABS,ACHAR,ACOS,ADJUSTL,ADJUSTR,AIMAG,AINT,ALL,ALLOCATE,ALLOCATED,ANINT,ANY,ASIN,ATAN,ATAN2,BACKSPACE,BIT_SIZE,BLOCK,BREAK,BTEST,CALL,CARG,CASE,CEILING,CHAR,CLOSE,CMPLX,CONJG,CONTAINS,CONTINUE,COS,COSH,COUNT,CPU_TIME,CSHIFT,CYCLE,DATE_AND_TIME,DBLE,DEALLOCATE,DEFAULT,DIGITS,DIM,DO,DOT_PRODUCT,DPROD,DVCHK,ELSE,ELSEIF,ELSEWHERE,END,ENDDO,ENDFILE,ENDIF,ENDWHERE,ENTRY,EOSHIFT,EPSILON,ERROR,EXIT,EXP,EXPONENT,FLOOR,FLUSH,FORMAT,FRACTION,FUNCTION,GETCL,GO,GOTO,HUGE,IACHAR,IAND,IBCLR,IBITS,IBSET,ICHAR,IEOR,IF,INDEX,INQUIRE,INT,INTERFACE,INTRUP,INVALOP,IOR,IOSTAT_MSG,ISHFT,ISHFTC,LBOUND,LEN,LEN_TRIM,LGE,LGT,LLE,LLT,LOG,LOG10,LOOP,MAP,MATMUL,MAX,MAXEXPONENT,MAXLOC,MAXVAL,MERGE,MIN,MINEXPONENT,MINLOC,MINVAL,MOD,MODULE,MODULO,MVBITS,NBREAK,NDPERR,NDPEXC,NEAREST,NINT,NOT,NULLIFY,OFFSET,ONLY,OPEN,OPERATOR,OTHERWISE,OVEFL,PACK,PRECFILL,PRECISION,PRESENT,PRINT,PROCEDURE,PRODUCT,PROGRAM,PROMPT,QUIT,RADIX,RANDOM_NUMBER,RANDOM_SEED,RANGE,READ,RECORD,RECURSIVE,REMOTE,REPEAT,RESHAPE,RETURN,REWIND,RRSPACING,SCALE,SCAN,SEGMENT,SELECT,SELECTED_INT_KIND,SELECTED_REAL_KIND,SET_EXPONENT,SHAPE,SIGN,SIN,SINH,SIZE,SPACING,SPREAD,SQRT,STOP,STRUCTURE,SUBROUTINE,SUM,SYSTEM,SYSTEM_CLOCK,TAN,TANH,THEN,TIMER,TINY,TO,TRANSFER,TRANSPOSE,TRIM,UBOUND,UNDFL,UNION,UNPACK,UNTIL,VAL,VERIFY,WHERE,WHILE,WRITE,.AND.,.EQ.,.EQV.,.FALSE.,.GE.,.GT.,.LE.,.LT.,.NE.,.NEQV.,.NOT.,.OR.,.TRUE.,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_FORTRAN"), strAllSynWord);
		break;
	case _HLBL:
		strAllSynWord = _T(" AND,ARRAY,AS,ASM,BEGIN,BIT_PATTERN,CALL,CASE,CLASS,CONST,CONSTANTS,DEPENDENT_SEQUENCE,DIV,DO,DOWNTO,ELSE,ELSEIF,END,ENDFOR,ENDIF,ENDSUBROUTINE,FALSE,FOR,FUNCTION,GOTO,IF,IN,MOD,NOT,OR,PROCEDURE,STATEMENTS,SUBROUTINE,THEN,TRUE,USER_LABELS,VARIABLES,WAIT,WHILE,WITH,XOR,#define,#ifdef,#include,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_HLBL"), strAllSynWord);
		break;
//*
	case _HTML:
		strAllSynWord = _T(" center,codebase,quality,pluginspage,embed,bottomMargin,rightMargin,rel,frameset,frame,noresize,noframes,big,onkeypress,layer,ul,shape,area,coords,map,tppabs,frameborder,ilayer,nolayer,visibility,scrolling,onclick,onmouseout,Onmouseover,accesskey,for,noshade,maxlength,span,tabindex,div,value,onchange,label,public,doctype,leftmargin,topmargin,marginwidth,marginheight,meta,content,http-equiv,language,onLoad,onUnload,color,target,IFRAME,NOSCRIPT,class,spacer,A,tbody,face,vAlign,action,address,align,alink,alt,applet,b,background,base,basefont,bgcolor,blink,blockquote,body,border,br,caption,cellpadding,cellspacing,cite,classid,clear,code,cols,colspan,dd,dfn,dir,dl,dt,em,font,form,h1,h2,h3,h4,h5,h6,head,height,hr,href, special,hspace,html,i,id,img,input,isindex,ismap,kbd,li,link,link,listing,menu,method,multiple,name,nobr,nowrap,object,ol,option,p,param,pre,ref,rev,rows,rowspan,runat,samp,script,select,size,src,strike,strong,style,table,td,text,textarea,th,title,tr,tt,type,u,var,vlink,vspace,wbr,width,xmp,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_HTML"), strAllSynWord);
		break;
//*/
	case _INI:
		strAllSynWord = _T("");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_INI"), strAllSynWord);
		break;
	case _JAVA:
		strAllSynWord = _T(" abstract,boolean,break,byte,case,catch,char,class,continue,default,do,double,else,extends,false,final,finally,float,for,if,implements,import,instanceof,int,interface,length,long,native,new,null,package,private,protected,public,return,short,static,super,switch,synchronized,this,threadsafe,throw,throws,transient,true,try,void,while,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_JAVA"), strAllSynWord);
		break;
	case _JSP:
		strAllSynWord = _T(" abstract,boolean,break,byte,case,catch,char,class,const,continue,default,do,double,else,extends,false,final,finally,float,for,goto,if,implements,import,instanceof,int,interface,long,native,new,null,package,private,protected,public,return,short,static,strictfp,super,switch,synchronized,this,threadsafe,throw,throws,transient,true,try,void,volatile,while,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_JSP"), strAllSynWord);
		break;
	case _LIMBO:
		strAllSynWord = _T(" adt,alt,break,case,continue,cyclic,do,else,exit,fn,for,hd,if,implement,import,int,len,load,module,of,or,pick,return,self,spawn,tagof,tl,to,type,while,array,big,byte,chan,con,int,list,NIL,real,ref,string,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_LIMBO"), strAllSynWord);
		break;
	case _LISP:
		strAllSynWord = _T(" ABS,ADD1,AND,APPEND,APPLY,ARG,ARRAY,ASSOC,ATAN,ATOM,BAKTRACE,BOOLE,BOUNDP,BREAK,CAR,CDR,COMMENT,COND,CONS,COS,DEFPROP,DEFUN,DELETE,DELQ,DIFFERENCE,EQ,EQUAL,EVAL,EXP,EXPLODE,EXPT,FIX,FLOAT,FUNCALL,FUNCTION,GCD,GENSYM,GET,GO,GREATERP,IMPLODE,LAST,LENGTH,LESSP,LIST,LOAD,LOG,LSH,MAPCAN,MAPCAR,MAX,MEMBER,MIN,MINUS,MINUSP,NCONC,NOT,NULL,NUMBERP,OR,PLUS,PRIN1,PRINC,PRINT,PROG,PROG2,PROGN,PUTPROP,QUIT,QUOTE,QUOTIENT,READ,READCH,REMAINDER,REMPROP,RETURN,REVERSE,RPLACA,RPLACD,SET,SETQ,SIN,SORT,SORTCAR,SQRT,STORE,SUB1,SUBST,TERPRI,TIMES,TRACE,UNTRACE,ZEROP,close,EXPR,FEXPR,IF,LAMBDA,LEXPR,MACRO,open,strcase,strcat,THEN,WHILE,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_LISP"), strAllSynWord);
		break;
	case _MODULA2:
		strAllSynWord = _T(" and,begin,by,case,const,definition,div,do,else,elsif,end,exit,export,for,from,if,implementation,import,in,loop,mod,module,not,of,or,pointer,procedure,qualified,record,repeat,return,set,then,to,type,until,var,while,with,ARRAY,BOOLEAN,CARDINAL,CHAR,FLOAT,INTEGER,LONG,LONGCARD,LONGFLOAT,LONGINT,LONGORD,LONGREAL,LONGTRUNC,REAL,SHORT,Abs,Bitset,Cap,Chr,Dec,Dispose,Excl,Halt,High,Inc,Incl,Max,Min,New,Odd,Ord,Size,Trunc,Val,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_MODULA2"), strAllSynWord);
		break;
	case _OBJECTPASCAL:
		strAllSynWord = _T(" Absolute,Abstract,And,Array,As,Asm,Assembler,Automated,Begin,Case,Cdecl,Class,Const,Constructor,Contains,Default,Destructor,Dispid,Dispinterface,Div,Do,Downto,Dynamic,Else,End,Except,Export,Exports,External,Far,File,Finalization,Finally,For,Forward,Function,Goto,If,Implementation,Implements,In,Index,Inherited,Initialization,Inline,Interface,Is,Label,Name,Near,Message,Mod,Nil,Nodefault,Not,Object,Of,Or,Out,Overload,Override,Packed,Pascal,Private,Procedure,Program,Property,Protected,Protected,Public,Published,Raise,Read,Readonly,Record,Register,Reintroduce,Repeat,Resident,Resourcestring,Requires,Safecall,Set,Shl,Shr,Stdcall,Stored,String,Then,Threadvar,To,Try,Type,Unit,Until,Uses,Var,Virtual,While,With,Write,Writeonly,Xor,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_OBJECTPASCAL"), strAllSynWord);
		break;
	case _PARADOX:
		strAllSynWord = _T(" and,as,case,caseInsensitive,create,Database,descending,disableDefault,doDefault,else,enableDefault,endCreate,endFor,endForEach,endif,endIndex,endMethod,endProc,endQuery,endRecord,endScan,endSort,endSwitch,endSwitchMenu,endTry,endType,endUses,endWhile,for,forEach,from,if,iif,in,index,indexStruct,is,key,like,loop,maintained,method,NOT,ObjectPAL,of,on,onFail,or,otherwise,passEvent,primary,proc,query,quitLoop,record,refIntStruct,retry,return,scan,secStruct,sort,step,struct,switch,switchMenu,tag,then,to,try,type,unique,uses,where,while,with,without,AnyType,Array,Binary,const,Currency,Date,DateTime,DynArray,endconst,endvar,Graphic,Logical,LongInt,Memo,Number,OLE,Point,SmallInt,String,Time,var,action,ActionEvent,canArrive,canDepart,changeValue,close,error,ErrorEvent,Event,keyChar,KeyEvent,keyPhysical,menuAction,MenuEvent,mouseClick,mouseDouble,mouseDown,mouseEnter,MouseEvent,mouseExit,mouseMove,mouseRightDouble,mouseRightDown,mouseRightUp,mouseUp,newValue,open,pushButton,removeFocus,setFocus,status,StatusEvent,timer,TimerEvent,ValueEvent,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_PARADOX"), strAllSynWord);
		break;
	case _PERL:
		strAllSynWord = _T(" __DATA__,__END__,__FILE__,__LINE__,accept,alarm,atan2,AUTOLOAD,BEGIN,bind,binmode,bless,caller,chdir,chmod,chomp,chop,chown,chr,chroot,close,closedir,connect,continue,CORE,cos,crypt,dbmclose,dbmopen,defined,delete,DESTROY,die,do,dump,each,else,elsif,END,endgrent,endhostent,endnetent,endprotoent,endpwent,endservent,eof,eq,eval,exec,exit,exp,fcntl,fileno,flock,for,foreach,fork,format,formline,getc,getgrent,getgrgid,getgrname,gethostbyaddr,gethostbyname,gethostent,getlogin,getnetbyaddr,getnetbyname,getnetent,getpeername,getpgrp,getppid,getpriority,getprotobyname,getprotobynumber,getprotoent,getpwent,getpwnam,getpwuid,getservbyname,getservbyport,getservent,getsockname,getsockopt,glob,gmtime,goto,grep,hex,if,index,int,ioctl,join,keys,kill,last,lc,lcfirst,length,link,listen,local,localtime,log,lstat,map,mkdir,msgctl,msgget,msgrcv,msgsnd,my,next,no,not,oct,open,opendir,or,ord,pack,package,pipe,pop,pos,print,printf,push,quotemeta,rand,read,readdir,readline,readlink,recv,redo,ref,rename,require,reset,return,reverse,rewinddir,rindex,rmdir,scalar,seek,seekdir,select,semctl,semgett,semop,send,setgrent,sethostent,setnetent,setpgrp,setpriority,setprotoent,setpwent,setservent,setsockopt,shift,shmctl,shmget,shmread,shmwrite,shutdown,sin,sleep,socket,socketpair,sort,splice,split,sprintf,sqrt,srand,stat,study,sub,substr,SUPER,symlink,syscall,sysopen,sysread,system,syswrite,tell,telldir,tie,tied,time,times,truncate,uc,ucfirst,umask,undef,UNIVERSAL,unless,unlink,unpack,unshift,until,use,utime,values,vec,wait,waitpid,wantarray,warn,while,write,xor,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_PERL"), strAllSynWord);
		break;		
	case _PHP:
		strAllSynWord = _T(" __FILE__,__LINE__,BOOL,BREAK,CASE,CHAR,CLASS,CONTINUE,DEFAULT,DO,DOUBLE,E_ERROR,E_NOTICE,E_PARSE,E_WARNING,ELSE,ELSEIF,ENDFOR,ENDIF,ENDWHILE,EXTENDS,FALSE,FLOAT,FOR,FUNCTION,GLOBAL,IF,INCLUDE,INT,INTEGER,LONG,MIXED,NEW,OBJECT,OLD_FUNCTION,PHP,PHP_OS,PHP_VERSION,REAL,REQUIRE,RETURN,STATIC,STRING,SWITCH,TRUE,VAR,VOID,WHILE,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_PHP"), strAllSynWord);
		break;		
	case _PL1:
		strAllSynWord = _T(" act,activate,alloc,allocate,begin,call,check,close,dcl,deact,deactivate,declare,declare,default,delay,delete,dft,display,do,do,else,end,end,entry,exit,fetch,format,free,get,go,goto,goto,halt,if,if,include,locate,on,open,page,print,proc,proc,procedure,procedure,process,put,read,release,return,revert,rewrite,select,signal,skip,stop,to,unlock,until,wait,while,write,area,auto,automatic,backward,based,bin,binary,bit,bkwd,buf,buffered,builtin,char,character,complex,cond,condition,conn,connected,controlled,cplx,ctl,dec,decimal,def,defined,direct,entry,env,environment,event,excl,exclusive,ext,external,file,fixed,float,generic,init,initial,input,int,internal,irred,irreducible,keyed,label,like,offset,output,pic,picture,pointer,pos,position,print,ptr,real,record,recursive,red,reducible,returns,sequential,static,stream,task,transient,unal,unaligned,unbuf,unbuffered,update,var,variable,varying, abs,acos,add,addr,all,allocation,allocn,any,asin,atan,atand,atanh,binary,bit,bool,ceil,ceil,char,compiletime,completion,complex,conjg,cos,cosd,cosh,count,counter,datafield,date,decimal,dim,divide,empty,erf,erfc,exp,fixed,float,floor,floor,hbound,high,imag,index,lbound,length,lineno,log,log10,log2,low,max,max,min,min,mod,mod,multiply,null,offset,onchar,oncode,oncount,onfile,onkey,onloc,onsource,pointer,poly,prec,precision,priority,prod,real,repeat,round,round,samekey,sign,sign,sin,sind,sinh,sqrt,status,string,substr,sum,sum,tan,tand,tanh,time,translate,trunc,trunc,unspec,verify,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_PL1"), strAllSynWord);
		break;		
	case _PROGRESS:
		strAllSynWord = _T(" BIN,BYTE,CHAR,CHARACTER,COM-HANDLE,DATE,DEC,DECIMAL,DOUBLE,FLOAT,HANDLE,INT,INTEGER,LOG,LOGICAL,LONG,MEMPTR,RAW,RECID,ROWID,SHORT,UNSIGNED-SHORT,WIDGET,WIDGET-HANDLE,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_PROGRESS"), strAllSynWord);
		break;		
	case _PYTHON:
		strAllSynWord = _T(" and,assert,break,class,continue,def,del,elif,else,except,exec,finally,for,from,global,if,import,in,is,lambda,None,not,or,pass,print,raise,return,try,while,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_PYTHON"), strAllSynWord);
		break;		
	case _REBOL:
		strAllSynWord = _T(" about,absolute,add,alias,all,and,any,any-block!,any-block?,any-function!,any-function?,any-string!,any-string?,any-type!,any-type?,any-word!,any-word?,append,arccosine,arcsine,arctangent,array,ask,at,back,binary!,binary?,bind,bitset!,bitset?,block!,block?,break,catch,change,change-dir,char!,char?,charset,checksum,clear,close,comment,complement,compress,copy,cosine,datatype!,datatype?,date!,date?,decimal!,decimal?,decompress,delete,detab,dir?,disarm,divide,do,echo,either,email!,email?,empty?,entab,equal?,error!,error?,even?,exists?,exit,exp,false,fifth,file!,file?,find,first,for,forall,foreach,form,forskip,found?,fourth,func,function,function!,function?,get,get-word!,get-word?,greater?,greater-or-equal?,halt,hash!,hash?,head,head?,help,if,in,index?,info?,input,input?,insert,integer!,integer?,issue!,issue?,join,last,length?,lesser?,lesser-or-equal?,license,list!,list?,list-dir,lit-word!,lit-word?,load,log-10,log-2,log-e,logic!,logic?,loop,lowercase,make,make-dir,maximum,minimum,modified?,mold,money!,money?,multiply,native!,native?,negate,negative?,newline,next,");
		strAllSynWord+= _T("no,none,none!,none?,not,not-equal?,now,number!,number?,object!,object?,odd?,op!,op?,open,or,paren!,paren?,parse,path!,path?,pick,poke,port!,port?,positive?,power,prin,print,probe,query,quit,random,read,recycle,reduce,refinement!,refinement?,reform,rejoin,remainder,remold,remove,rename,repeat,return,same?,save,script?,second,secure,select,send,series!,series?,set,set-net,set-path!,set-path?,set-word!,set-word?,sine,size?,skip,sort,source,square-root,strict-equal?,strict-not-equal?,string!,string?,subtract,symbol!,symbol?,system,tag!,tag?,tail,tail?,tangent,third,throw,time!,time?,to-binary,to-bitset,to-block,to-char,to-date,to-decimal,to-email,to-file,to-get-word,to-hash,to-hex,to-idate,to-integer,to-issue,to-list,to-lit-word,to-logic,to-money,to-none,to-paren,to-refinement,to-set-path,to-set-word,to-string,to-tag,to-time,to-tuple,to-url,to-word,trace,trim,true,try,tuple!,tuple?,type?,unset,unset?,until,update,uppercase,url!,url?,usage,use,value?,wait,what,what-dir,while,word!,word?,write,xor,yes,zero?,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_REBOL"), strAllSynWord);
		break;		
	case _REXX:
		strAllSynWord = _T(" address,arg,by,call,digits,do,drop,else,end,engineering,exit,expose,for,forever,form,fuzz,if,interpret,iterate,leave,linein,name,nop,numeric,off,on,options,otherwise,parse,procedure,pull,push,queue,return,say,scientific,select,signal,source,then,to,trace,until,upper,value,var,version,when,while,with,abbrev,abs,address,arg,b2x,bitand,bitor,bitxor,c2d,c2x,center,charin,charout,chars,compare,condition,copies,d2c,d2x,datatype,date,delstr,delword,digits,errortext,form,format,fuzz,insert,lastpos,left,length,linein,lineout,lines,max,min,overlay,pos,queued,random,reverse,right,sign,sourceline,space,stream,strip,substr,subword,symbol,time,trace,translate,trunc,value,verify,word,wordindex,wordlength,wordpos,words,x2b,x2c,x2d,xrange,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_REXX"), strAllSynWord);
		break;		
	case _RUBY:
		strAllSynWord = _T(" alias,and,begin,BEGIN,break,case,class,def,defined,do,else,elsif,end,END,ensure,false,for,if,in,module,next,nil,not,or,redo,rescue,retry,return,self,super,then,true,undef,unless,until,when,while,yield,__FILE__,__LINE__,Array,Float,Integer,String,abort,at_exit,autoload,binding,caller,callcc,catch,chop,chop!,chomp,chomp!,eval,exec,exit,exit!,fork,gets,readline,global_variables,gsub,gsub!,iterator?,block_given?,load,local_variables,loop,open,p,print,printf,proc,lambda,putc,puts,raise,fail,rand,readlines,require,scan,select,set_trace_func,sleep,split,sprintf,format,srand,sub,sub!,syscall,system,test,throw,trace_var,trap,untrace_var,TRUE,FALSE,NIL,STDIN,STDOUT,STDERR,ENV,ARGF,ARGV,DATA,TOPLEVEL_BINDING,RUBY_VERSION,RUBY_RELEASE_DATE,RUBY_PLATFORM,VERSION,RELEASE_DATE,PLATFORM,Object,Array,Binding,Continuation,Data,Exception,Dir,FalseClass,File,Hash,IO,File,MatchData,Method,UnboundMethod,Module,Class,Numeric,Integer,Bignum,Fixnum,Float,Proc,Process,Range,Regexp,String,Struct,Symbol,Thread,ThreadGroup,Time,TrueClass,NilClass,Object,Exception,Interrupt,NoMemoryError,ScriptError,LoadError,NameError,NotImplementedError,SyntaxError,SignalException,Interrupt,StandardError,ArgumentError,IndexError,IOError,EOFError,LocalJumpError,NoMethodError,RangeError,FloatDomainError,RegexpError,RuntimeError,SecurityError,SystemCallError,Errno,SystemStackError,ThreadError,TypeError,ZeroDivisionError,SystemExit,fatal,Comparable,Enumerable,Errno,FileTest,GC,Kernel,Marshal,Math,ObjectSpace,Precision,Process,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_RUBY"), strAllSynWord);
		break;		
	case _SMALLTALK:
		strAllSynWord = _T(" and,at,collect,do,ifFalse,ifTrue,or,put,reject,select,to,whileFalse,whileTrue,with,Array,Bag,Character,FCB,LargeNegativeInteger,LargePositiveInteger,Point,SmallInteger,String,SwappedOutObject,Symbol,UndefinedObject,abs,add,add:withOccurrences,addAll,addAllFirst,addAllLast,addFirst,addLast,allMask,and,anyMask,arcCos,arcSin,arcTan,asArray,asBag,asCharacter,asciiValue,asDictionary,asFloat,asInteger,asList,asLowercase,asSet,asString,asSymbol,asUppercase,at,at:ifAbsent,at:put,atAll:put,between:and,binaryDo,bitAnd,bitAt,bitInvert,bitOr,bitShift,bitXor,block,ceiling,checkBucket,class,clearScreen,coerce,collect,compareError,copy,copyFrom:length,copyFrom:to,copyWith,copyWithout,cos,current,currentKey,date,debug,deepCopy,detect,detect:ifAbsent,digitValue,display,displayAssign,dist,do,doPrimitive:withArguments,edit,equals:startingAt,eqv,error,even,exp,factorial,false,findAssociation:inList,findFirst,findFirst:ifAbsent,findLast,findLast:ifAbsent,first,firstKey,floor,fork,forkWith,fractionPart,from:to:by,gamma,gcd,getList,getString,grow,hashNumber,highBit,ifFalse,ifFalse:ifTrue,ifTrue,ifTrue:ifFalse,includes,includesKey,indexOf,indexOf:ifAbsent,");
		strAllSynWord+= _T(" indexOfSubCollection:startingAt,indexOfSubCollection:startingAt:ifAbsent,inject:into,inRange,integerPart,isAlphaNumeric,isBits,isBytes,isCharacter,isDigit,isEmpty,isKindOf,isLetter,isLowercase,isMemberOf,isNil,isSeparator,isUppercase,isVariable,isVowel,keys,keysDo,keysSelect,last,lastKey,lcm,list,ln,log,max,maxtype,min,modeCharacter,modeInteger,modeString,negated,negative,new,newProcess,newProcessWith,next,nil,noDisplay,noMask,not,nothing,notNil,occurrencesOf,odd,open,open:for,or,padTo,pen,perform:withArguments,pi,positive,print,printAt,printNoReturn,printString,prompt,quo,radians,radix,raisedTo,randInteger,randomize,read,reciprocal,reject,rem,remove,remove:ifAbsent,removeError,removeFirst,removeKey,removeKey:ifAbsent,removeLast,replaceFrom:to:with,replaceFrom:to:with:startingAt,respondsTo,resume,reversed,reverseDo,rounded,roundTo,sameAs,select,sh,shallowCopy,sign,signal,sin,size,sort,sqrt,squared,state,strictlyPositive,superClass,suspend,tan,termErr,terminate,time,timesRepeat,to,to:by,transpose,true,truncated,truncateTo,unblock,value,values,variables,view,wait,whileFalse,whileTrue,with:do,write,x,x:y,xor,y,yield,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_SMALLTALK"), strAllSynWord);
		break;		
	case _SQL:
		strAllSynWord = _T(" 1CE,1OL,ABS,AFTER,ALTER,AND,AS,AVG,BEFORE,BEGIN,BETWEEN,BODY,BREAK,BUFFER,BY,CHAR,CHAR,CLEAR,CLOSE,CLUSTER,COMMIT,COMPUTE,CONSTANT,CONSTRAINT,COS,COSH,COUNT,CREATE,CURSOR,DATABASE,DATAFILE,DATATYPE,DATE,DB1CE,DECIMAL,DECLARE,DEFAULT,DEFINE,DELETE,DROP,DUMP,ECHO,ELSE,ELSIF,END,EXCEPTION,EXCEPTION_INIT,EXECUTE,EXIT,EXP,EXPLAIN,FEEDBACK,FETCH,FLUSH,FOR,FORMAT,FROM,FROM,FUNCTION,GOTO,GRANT,GROUP,HEADING,HEXTORAW,IF,IN,INDEX,INDEX,INITIAL,INSERT,INSERT,INTEGER,INTO,INTO,IS,LEFT,LENGTH,LIKE,LINESIZE,LOCKMODE,LONG,LOOP,LTRIM,LUB,MAX,MAXEXTENTS,MIN,MINEXTENTS,MOD,MONEY,NEWPAGE,NEXT,NO_DATA_FOUND,NOT,NULL,NUMBER,OF,OFF,ON,OPEN,OR,ORDER,OTHERS,PACKAGE,PAGESIZE,PAUSE,PCTINCREASE,PROCEDURE,PROCEDURE,PROMPT,PUBLIC,RAISE,REAL,RENAME,REPLACE,RETURN,REVOKE,ROUND,RTRIM,SCHEMA,SELECT,SELECT,SERVEROUT,SET,SIN,SIZE,SORT,START,STORAGE,SUM,SYNONYM,TAB,TABLE,TABLE,TABLE1CE,TAN,TEMP,TERMOUT,THEN,TIME,TIMING,TRANSACTION,TRIGGER,TYPE,UNION,UPDATE,UPDATE,UPPER,VARCHAR2,VARCHAR2,VERIFY,VIEW,WHEN,WHERE,WHILE,WRAP,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_SQL"), strAllSynWord);
		break;		
	case _VHDL:
		strAllSynWord = _T(" abs,access,after,alias,all,and,architecture,array,assert,attribute,begin,block,body,buffer,bus,case,component,configuration,constant,disconnect,downto,else,elsif,end,entity,exit,file,for,function,generate,generic,group,guarded,if,impure,in,inertial,inout,is,label,library,linkage,literal,loop,map,mod,nand,new,next,nor,not,null,of,on,open,or,others,out,package,port,postponed,procedure,process,pure,range,record,register,reject,rem,report,return,rol,ror,select,severity,shared,signal,sla,sll,sra,srl,subtype,then,to,transport,type,unaffected,units,until,use,variable,wait,when,while,with,xnor,xor,is_x,resolved,rising_edge,std_logic,std_logic_vector,std_ulogic,std_ulogic_vector,t_ux01z,t_x01,t_x01z,to_bit,to_bitvector,to_stdlogicvector,to_stdulogic,to_stdulogicvector,ux01z,x01,x01z,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_VHDL"), strAllSynWord);
		break;		
	case _XML:
		strAllSynWord = _T(" !DOCTYPE,!ENTITY,!ELEMENT,!ATTLIST,!NOTATION,CDATA,ID,IDREF,IDREFS,ENTITY,ENTITIES,NMTOKEN,NMTOKENS,^#REQUIRED,^#IMPLIED,^#FIXED,");
		strAllSynWord = app->GetProfileString(_T("Keywords"), _T("_XML"), strAllSynWord);
		break;		
	default: //is NONE
		return;
	}

	if(!g_bCase) {
		strAllSynWord.MakeLower(); 
	}

	CString strTemp;
	int nPosPrior = 0;
	int nPos;	

	nPos = strAllSynWord.Find(_T(","), nPosPrior);
	while(nPos!=-1)
	{
		strTemp = strAllSynWord.Mid(nPosPrior+1 , nPos - nPosPrior - 1);
		m_strArrayKeyWords.Add(strTemp);
		
		nPosPrior = nPos;
		nPos = strAllSynWord.Find(_T(","),  nPosPrior + 1);			
	}
	SortSynWordArray(m_strArrayKeyWords);
}

/*还有点小问题
//希尔排序
void SortSynWordArray(CStringArray &m_strArrayKeyWords)
{
	CString strtmp;
	int nCount = m_strArrayKeyWords.GetSize(); 
	int k, j, i, cmpret;		
	k=nCount/2;
	while(k>=1)
	{
		for(i=k;i<nCount;i++)
		{
			strtmp = m_strArrayKeyWords.GetAt(i); 
			j=i-k;
			cmpret = strtmp.Compare(m_strArrayKeyWords.GetAt(j));
			while(j>=0 && cmpret<=0)
			{
				m_strArrayKeyWords.SetAt(j+k, m_strArrayKeyWords.GetAt(j));
				j=j-k;
			}
			m_strArrayKeyWords.SetAt(j+k, strtmp);
		}
		k/=2;
	}
}
//*/

void SortSynWordArray(CStringArray &m_strArrayKeyWords)
{
	int i,j,x, count = m_strArrayKeyWords.GetSize(); 
	CString strtmp;
	for(i=1;i<count;i++)
	{
		for(x=0;x<i;x++)
		{
			if(m_strArrayKeyWords.GetAt(i).Compare(m_strArrayKeyWords.GetAt(x))<=0)
				break;
		}
		strtmp=m_strArrayKeyWords.GetAt(i);
		for(j=i;j>x;j--)
			m_strArrayKeyWords.SetAt(j, m_strArrayKeyWords.GetAt(j-1));
		m_strArrayKeyWords.SetAt(x, strtmp);
	}
}


/*///////////////////////////////////////////////////////////////////////////////////////////////
ParseLine(m_strArrayKeyWords, dwCookie, strLine, ColorInfo, nActualItems) );
功能：根据所给定的语言解析一行字符串，返回用颜色指示的字符串块，以及解析后的当前行的解析结果，
指示解析后当前行处于什么状态，是处于字符串中、注释中、多行注释中还是正常状态。
入口参数：
m_strArrayKeyWords: 关键字列表
dwCookie:           传入前一行的解析结果，如果前一行是多行注释的开始或者前一行是一软回车时有用
strLine:            被解析的一行字符串
ColorInfo:          解析后的颜色值，指定是关键字、字符串、注释还是正常的字符颜色
nActualItems        strLine被解析后的块数
/////////////////////////////////////////////////////////////////////////////////////////////////*/
DWORD ParseLine(CStringArray &m_strArrayKeyWords, 
					  DWORD dwCookie, 
					  CString &strLine, 
					  COLORINFO *ColorInfo, 
					  int &nActualItems)
{
	//*
	if(g_nCurLanguage ==_HTML || g_nCurLanguage ==_XML) {
		int dwCookieTmp = ParseLineForHtml(m_strArrayKeyWords, dwCookie, strLine, ColorInfo, nActualItems);
		return dwCookieTmp;
	}
//*/
	int nLength = strLine.GetLength(); 
	int nIdentBegin = -1;
	int i;
	BOOL bMatch;
	BOOL bInString; //定义一个字符串的界限
	BOOL bRedefineBlock = TRUE;
	BOOL bDecIndex  = FALSE;
	BOOL bIsInTheControlChar = FALSE; //指示是否在\的控制中, strControl为空时无效, 初始化时必须为FALSE, 原因见结束定义字符串的处理代码
	if(nLength == 0)
		return dwCookie;
int I = 0;
	for ( I = 0; ; I++)
	{
		if (bRedefineBlock) //如果开始定义颜色块向下做
		{
			int nPos = I;
			if (bDecIndex)
				nPos--;
			// 是注释
			if (dwCookie & (COOKIE_COMMENT | COOKIE_EXT_COMMENT))
			{
				DEFINE_BLOCK(nPos, COLORINDEX_COMMENT);
			}
			else {
				// 是字符串
				if (dwCookie & COOKIE_STRING)
				{
					DEFINE_BLOCK(nPos, COLORINDEX_STRING);
				}
				else // 普通情况
				{
					DEFINE_BLOCK(nPos, COLORINDEX_NORMAL);
				}
			}
			bRedefineBlock = FALSE;
			bDecIndex      = FALSE;
		}
		
		if (I >= nLength)
			goto Out;
		
		//遇到双字节字体如中文直接跳过
		if(IsDBCSLeadByte(strLine[I])) //检查是不是中文字符
		{
			I += 2;
			if (I >= nLength)
				goto Out; // 判断是不是注释
			else
				goto cmpnextchar; // 无视,继续处理strLine的下一个字符
		}
		//////////////////////////////
		
		if (dwCookie & COOKIE_COMMENT)
		{
			DEFINE_BLOCK(I, COLORINDEX_COMMENT);
			dwCookie |= COOKIE_COMMENT;
			goto Out;
		}
		
		//如果当前正处于字符串变量中，则应检查是否有结束字符串变量的字符串
		if (dwCookie & COOKIE_STRING)
		{
			if(g_bControl) { //处理控制字符，至少C++会用到(例\"表示引号而不是两个字符)
				if(I > 0) {
					if(strLine[I-1] != '\\') {
						if(strLine[I] != '\\') 
							bIsInTheControlChar = FALSE;
						else
							bIsInTheControlChar = TRUE;
					}
					else {
						if(strLine[I] == '\\') {
							if(!bIsInTheControlChar)
								bIsInTheControlChar = TRUE;
							else
								bIsInTheControlChar = FALSE;
						}
					}
				}
			}
			
			int nlen;
			for (int i = 0; i < g_nEndStringStrCount; i++ ) { 
				if(g_EndStringStr[i]==NULL)
					continue;
				nlen = /*strlen*/strlen(g_EndStringStr[i]);
				bMatch = TRUE;
				//如果strLine的当前字符串值与结束定义字符规则中的一项相同则中止定义字符串
				if (I >= nlen - 1)
				{
					for(int k=0; k<nlen; k++) { //向前搜索字符串检查是否与结束定义的字符串相同
						if(strLine[I-nlen+1+k]!=g_EndStringStr[i][k]) {
							bMatch = FALSE; //如果不匹配，中断比较
							break;
						}
					}
					if(bMatch && !bIsInTheControlChar) {
						dwCookie &= ~COOKIE_STRING;
						bRedefineBlock = TRUE;
						goto cmpnextchar; //继续处理strLine的下一个字符
					}
				}
			}
			goto cmpnextchar; //如果没有结束定义字符串继续处理strLine的下一个字符
		}
		
		//如果当前正处于字符变量中，则应检查是否有结束字符变量的字符串
		if (dwCookie & COOKIE_CHAR)
		{
			if(g_bControl) { //处理控制字符，至少C++会用到(例\"表示引号而不是两个字符)
				if(I > 0) {
					if(strLine[I-1] != '\\') {
						if(strLine[I] != '\\') 
							bIsInTheControlChar = FALSE;
						else
							bIsInTheControlChar = TRUE;
					}
					else {
						if(strLine[I] == '\\') {
							if(!bIsInTheControlChar)
								bIsInTheControlChar = TRUE;
							else
								bIsInTheControlChar = FALSE;
						}
					}
				}
			}
			
			int nlen;
			for (int i = 0; i < g_nEndCharStrCount; i++ ) { 
				if(g_EndCharStr[i]==NULL)
					continue;
				nlen = strlen(g_EndCharStr[i]);
				bMatch = TRUE;
				//如果strLine的当前字符值与结束定义字符规则中的一项相同则中止定义字符串
				if (I >= nlen - 1)
				{
					for(int k=0; k<nlen; k++) { //向前搜索字符串检查是否与结束定义的字符相同
						if(strLine[I-nlen+1+k]!=g_EndCharStr[i][k]) {
							bMatch = FALSE; //如果不匹配，中断比较
							break;
						}
					}
					if(bMatch && !bIsInTheControlChar) {
						dwCookie &= ~COOKIE_CHAR;
						bRedefineBlock = TRUE;
						goto cmpnextchar; //继续处理strLine的下一个字符
					}
				}
			}
			goto cmpnextchar; //如果没有结束定义字符继续处理strLine的下一个字符
		}

		//如果当前正处于多行注释中，则应检查是否有结束多行注释的字符串
		if (dwCookie & COOKIE_EXT_COMMENT)	{
			for (int i = 0; i < g_nEndCommentExStrCount; i++ )	{
				if(g_EndCommentExStr[i]==NULL)
					continue;
				bMatch = TRUE;
				int nlen = strlen(g_EndCommentExStr[i]); 
				if(I >= nlen - 1)
				{
					for(int k=0; k<nlen; k++) {
						if(strLine[I-nlen+1+k]!=g_EndCommentExStr[i][k]) {
							bMatch = FALSE;
							break;
						}
					}
					if(bMatch) {
						dwCookie &= ~COOKIE_EXT_COMMENT;
						bRedefineBlock = TRUE;
						goto cmpnextchar;
					}
				}
			}
			goto cmpnextchar;
		}
	
		//处理单行注释
		for (i = 0; i < g_nLineCommentStrCount; i++ )	{
			if(g_LineCommentStr[i]==NULL)
				continue;
			int nlen = strlen(g_LineCommentStr[i]); 
			bMatch = TRUE;
			if(I >= nlen - 1)
			{
				for(int k=0; k<nlen; k++) {
					if(strLine[I-nlen+1+k]!=g_LineCommentStr[i][k]) {
						bMatch = FALSE;
						break;
					}
				}
				if(bMatch) {
					DEFINE_BLOCK(I - nlen + 1, COLORINDEX_COMMENT);
					dwCookie |= COOKIE_COMMENT;
					goto Out; //只要发现有单行注释的字符串则退出
				}
			}
		}

		//处理字符串变量的开始
		for (i = 0; i < g_nBeginStringStrCount; i++ )	{
			if(g_BeginStringStr[i]==NULL)
				continue;
			int nlen = strlen(g_BeginStringStr[i]); 
			bMatch = TRUE;
			if(I >= nlen - 1)
			{
				for(int k=0; k<nlen; k++) {
					if(strLine[I-nlen+1+k]!=g_BeginStringStr[i][k]) {
						bMatch = FALSE;
						break;
					}
				}
				if(bMatch) {
					DEFINE_BLOCK(I - nlen + 1, COLORINDEX_STRING);
					dwCookie |= COOKIE_STRING;
					goto cmpnextchar;
				}
			}
		}

		//处理字符型变量的开始
		for (i = 0; i < g_nBeginCharStrCount; i++ )	{
			if(g_BeginCharStr[i]==NULL)
				continue;
			int nlen = strlen(g_BeginCharStr[i]); 
			bMatch = TRUE;
			if(I >= nlen - 1)
			{
				for(int k=0; k<nlen; k++) {
					if(strLine[I-nlen+1+k]!=g_BeginCharStr[i][k]) {
						bMatch = FALSE;
						break;
					}
				}
				if(bMatch) {
					DEFINE_BLOCK(I - nlen + 1, COLORINDEX_CHAR);
					dwCookie |= COOKIE_CHAR;
					goto cmpnextchar;
				}
			}
		}

		//处理多行注释的开始
		for (i = 0; i < g_nBeginCommentExStrCount; i++ ) {
			if(g_BeginCommentExStr[i]==NULL)
				continue;
			int nlen = strlen(g_BeginCommentExStr[i]); 
			bMatch = TRUE;
			if(I >= nlen - 1)
			{
				for(int k=0; k<nlen; k++) {
					if(strLine[I-nlen+1+k]!=g_BeginCommentExStr[i][k]) {
						bMatch = FALSE;
						break;
					}
				}
				if(bMatch) {
					DEFINE_BLOCK(I - nlen + 1, COLORINDEX_COMMENT);
					dwCookie |= COOKIE_EXT_COMMENT;
					if(nlen>1) {
						if ( ++I>= nLength) // 考虑"/*/"的情况
							goto Out;
					}
					goto cmpnextchar;
				}
			}
		}
		
		bInString = FALSE;
		for ( i = 0; i < g_nstrInStringCount; i++ )  {
			 BOOL b = ( strLine[I] == g_strInString.GetAt(i) ) ; 
			 bInString |= b;
			 if(bInString)
				 break;
		}
		if (isalnum(strLine[I]) || strLine[I] == '_' || bInString)
		{
			if (nIdentBegin == -1)
				nIdentBegin = I;
		}
		else
		{
			if (nIdentBegin >= 0)
			{
				CString strtmp= strLine.Mid(nIdentBegin, I - nIdentBegin); 
				if (IsSynWord(m_strArrayKeyWords, strtmp))
				{
					DEFINE_BLOCK(nIdentBegin, COLORINDEX_SYNTAX);
				}
				else if (IsNumber(strtmp))
				{
					DEFINE_BLOCK(nIdentBegin, COLORINDEX_NUMBER);
				}
				bRedefineBlock = TRUE;
				bDecIndex = TRUE;
				nIdentBegin = -1;
			}
		}
cmpnextchar:
		i=0;  //没有意义，为编译通过而准备
	}
	
Out: //当遇到单行注释时会直接跳到这儿
	if (nIdentBegin >= 0)
	{
		CString strtmp = strLine.Mid(nIdentBegin, I - nIdentBegin); 
		if (IsSynWord(m_strArrayKeyWords, strtmp))
		{
			DEFINE_BLOCK(nIdentBegin, COLORINDEX_SYNTAX);
		}
		else if (IsNumber(strtmp))
		{
			DEFINE_BLOCK(nIdentBegin, COLORINDEX_NUMBER);
		}
	}

	return dwCookie;
}


DWORD ParseLineForHtml(CStringArray &m_strArrayKeyWords, 
					  DWORD dwCookie, 
					  CString &strLine, 
					  COLORINFO *ColorInfo, 
					  int &nActualItems)
{	
	//对于无多行注释的语法在只取当前行的解析结果时可直接返回
	if(g_nBeginCommentExStrCount==0 && ColorInfo==NULL) 
		return dwCookie;

	int nLength = strLine.GetLength(); 
	int nIdentBegin = -1;
	int i;
	BOOL bMatch;
	BOOL bInString; //定义一个字符串的界限
	BOOL bRedefineBlock = TRUE;
	BOOL bDecIndex  = FALSE;

	if(nLength == 0)
		return dwCookie;
int I = 0;
	for ( I = 0; ; I++)
	{
		if (bRedefineBlock) //如果开始定义颜色块向下做
		{
			int nPos = I;
			if (bDecIndex)
				nPos--;
			if (dwCookie & COOKIE_EXT_COMMENT)
			{
				DEFINE_BLOCK(nPos, COLORINDEX_COMMENT);
			}
			else if (dwCookie & COOKIE_STRING)
			{
				DEFINE_BLOCK(nPos, COLORINDEX_STRING);
			}
			else if (dwCookie & COOKIE_SCRIPT)
			{
				DEFINE_BLOCK(nPos, COLORINDEX_STRING);
			}
			else
			{
				DEFINE_BLOCK(nPos, COLORINDEX_NORMAL);
			}
			bRedefineBlock = FALSE;
			bDecIndex      = FALSE;
		}
		
		if (I >= nLength)
			goto Out;		
		
		//遇到双字节字体如中文直接跳过
		if(IsDBCSLeadByte(strLine[I])) //检查是不是中文字符
		{
			I += 2;
			if (I >= nLength)
				goto Out;
			else
				goto cmpnextchar; //继续处理strLine的下一个字符
		}
		//////////////////////////////

		if ( !(dwCookie & COOKIE_TAG) ) 
		{
			if (dwCookie & COOKIE_SCRIPT) 
			{
				if ( I + 7 < nLength)
				{// the string "</script" length is 7;
					if (strLine[I] != '<')
						continue;
					if (strLine[I+1] != '/')
						continue;
					if (strLine[I+2] != 's' && strLine[I+2] != 'S')
						continue;
					if (strLine[I+3] != 'c' && strLine[I+3] != 'C')
						continue;
					if (strLine[I+4] != 'r' && strLine[I+4] != 'R')
						continue;
					if (strLine[I+5] != 'i' && strLine[I+5] != 'I')
						continue;
					if (strLine[I+6] != 'p' && strLine[I+6] != 'P')
						continue;
					if (strLine[I+7] != 't' && strLine[I+7] != 'T')
						continue;
					dwCookie &= ~COOKIE_SCRIPT;
					goto con;
				}
				continue;
			}
		}
con:
		if (strLine[I] == '<') 
		{
			dwCookie |= COOKIE_TAG;
			DEFINE_BLOCK(I, COLORINDEX_SYNTAX);
			//DEFINE_BLOCK(I+1, COLORINDEX_NORMAL);
		}				
		
		if (dwCookie & COOKIE_TAG) 
		{		
			//如果当前正处于字符串变量中，则应检查是否有结束字符串变量的字符串
			if ( dwCookie & COOKIE_STRING )
			{
				int nlen;
				for (int i = 0; i < g_nEndStringStrCount; i++ ) 
				{ 
					if(g_EndStringStr[i]==NULL)
						continue;
					nlen = strlen(g_EndStringStr[i]);
					bMatch = TRUE;
					//如果strLine的当前字符串值与结束定义字符规则中的一项相同则中止定义字符串
					if (I >= nlen - 1)
					{
						for(int k=0; k<nlen; k++) { //向前搜索字符串检查是否与结束定义的字符串相同
							if(strLine[I-nlen+1+k]!=g_EndStringStr[i][k]) 
							{
								bMatch = FALSE; //如果不匹配，中断比较
								break;
							}
						}
						if(bMatch) {
							dwCookie &= ~COOKIE_STRING;
							bRedefineBlock = TRUE;
							goto cmpnextchar; //继续处理strLine的下一个字符
						}
					}
				}
				goto cmpnextchar; //如果没有结束定义字符串继续处理strLine的下一个字符
			}
			
			//如果当前正处于字符变量中，则应检查是否有结束字符变量的字符串
			if ( dwCookie & COOKIE_CHAR )
			{
				int nlen;
				for (i = 0; i < g_nEndCharStrCount; i++ )
				{ 
					if(g_EndCharStr[i]==NULL)
						continue;
					nlen = strlen(g_EndCharStr[i]);
					bMatch = TRUE;
					//如果strLine的当前字符值与结束定义字符规则中的一项相同则中止定义字符串
					if (I >= nlen - 1)
					{
						for(int k=0; k<nlen; k++) { //向前搜索字符串检查是否与结束定义的字符相同
							if(strLine[I-nlen+1+k]!=g_EndCharStr[i][k]) 
							{
								bMatch = FALSE; //如果不匹配，中断比较
								break;
							}
						}
						if(bMatch) {
							dwCookie &= ~COOKIE_CHAR;
							bRedefineBlock = TRUE;
							goto cmpnextchar; //继续处理strLine的下一个字符
						}
					}
				}
				goto cmpnextchar; //如果没有结束定义字符继续处理strLine的下一个字符
			}
			
			//如果当前正处于多行注释中，则应检查是否有结束多行注释的字符串
			if ( dwCookie & COOKIE_EXT_COMMENT )	{
				for (i = 0; i < g_nEndCommentExStrCount; i++ )
				{
					if(g_EndCommentExStr[i]==NULL)
						continue;
					bMatch = TRUE;
					int nlen = strlen(g_EndCommentExStr[i]); 
					if(I >= nlen - 1)
					{
						for(int k=0; k<nlen; k++) {
							if(strLine[I-nlen+1+k]!=g_EndCommentExStr[i][k])
							{
								bMatch = FALSE;
								break;
							}
						}
						if(bMatch) {
							dwCookie &= ~COOKIE_EXT_COMMENT;
							bRedefineBlock = TRUE;
							goto cmpnextchar;
						}
					}
				}
				goto cmpnextchar;
			}
			
			//处理字符串变量的开始
			for (i = 0; i < g_nBeginStringStrCount; i++ )	{
				if(g_BeginStringStr[i]==NULL)
					continue;
				int nlen = strlen(g_BeginStringStr[i]); 
				bMatch = TRUE;
				if(I >= nlen - 1)
				{
					for(int k=0; k<nlen; k++) 
					{
						if(strLine[I-nlen+1+k]!=g_BeginStringStr[i][k]) 
						{
							bMatch = FALSE;
							break;
						}
					}
					if(bMatch) {
						DEFINE_BLOCK(I - nlen + 1, COLORINDEX_STRING);
						dwCookie |= COOKIE_STRING;
						goto cmpnextchar;
					}
				}
			}
			
			//处理字符型变量的开始
			for (i = 0; i < g_nBeginCharStrCount; i++ )
			{
				if(g_BeginCharStr[i]==NULL)
					continue;
				int nlen = strlen(g_BeginCharStr[i]); 
				bMatch = TRUE;
				if(I >= nlen - 1)
				{
					for(int k=0; k<nlen; k++) 
					{
						if(strLine[I-nlen+1+k]!=g_BeginCharStr[i][k])
						{
							bMatch = FALSE;
							break;
						}
					}
					if(bMatch) {
						DEFINE_BLOCK(I - nlen + 1, COLORINDEX_CHAR);
						dwCookie |= COOKIE_CHAR;
						goto cmpnextchar;
					}
				}
			}
			
			//处理多行注释的开始
			for (i = 0; i < g_nBeginCommentExStrCount; i++ ) 
			{
				if(g_BeginCommentExStr[i]==NULL)
					continue;
				int nlen = strlen(g_BeginCommentExStr[i]); 
				bMatch = TRUE;
				if(I >= nlen - 1)
				{
					for(int k=0; k<nlen; k++) {
						if(strLine[I-nlen+1+k]!=g_BeginCommentExStr[i][k]) 
						{
							bMatch = FALSE;
							break;
						}
					}
					if(bMatch) 
					{
						nActualItems--;
						DEFINE_BLOCK(I - nlen + 1, COLORINDEX_COMMENT);
						dwCookie |= COOKIE_EXT_COMMENT;
						goto cmpnextchar;
					}
				}
			}
			
			bInString = FALSE;
			for ( i = 0; i < g_nstrInStringCount; i++ ) 
			{
				BOOL b = ( strLine[I] == g_strInString.GetAt(i) );
				bInString |= b;
				if(bInString)
					break;
			}
			if (isalnum(strLine[I]) || strLine[I] == '_' || bInString)
			{
				if (nIdentBegin == -1)
					nIdentBegin = I;
			}
			else
			{
				if (nIdentBegin >= 0)
				{
					CString strtmp= strLine.Mid(nIdentBegin, I - nIdentBegin); 
					if (IsNumber(strtmp))
					{
						DEFINE_BLOCK(nIdentBegin, COLORINDEX_NUMBER);
					}//*
					else 
					{
						if(strtmp.CompareNoCase(_T("script"))==0) 
						{
							if( nIdentBegin > 0 && strLine[nIdentBegin-1] == '<') 
							{
								dwCookie |= COOKIE_SCRIPT;
							}
							else if( nIdentBegin > 1 && strLine[nIdentBegin-1] == '/' && strLine[nIdentBegin-2] == '<') 
							{
								dwCookie &= ~COOKIE_SCRIPT;
								DEFINE_BLOCK(nIdentBegin-2, COLORINDEX_SYNTAX);
							}
						}
						if( (nIdentBegin > 0) && (strLine[nIdentBegin-1] == '=') ) 
						{
							DEFINE_BLOCK(nIdentBegin, COLORINDEX_NORMAL);
						}
						else 
						{
							DEFINE_BLOCK(nIdentBegin, COLORINDEX_SYNTAX);
						}
					}
					bRedefineBlock = TRUE;
					bDecIndex = TRUE;
					nIdentBegin = -1;
				}
			}
		}
		if (strLine[I] == '>') 
		{
            bRedefineBlock = FALSE;
			dwCookie &= ~COOKIE_TAG;
			DEFINE_BLOCK(I, COLORINDEX_SYNTAX);
			if(dwCookie & COOKIE_SCRIPT) 
			{
				DEFINE_BLOCK(I+1, COLORINDEX_STRING);
			}
			else if( (I > 1) && (strLine[I-1]=='-') && (strLine[I-2]=='-') )
			{
				DEFINE_BLOCK(I, COLORINDEX_COMMENT);					
			}
			else
			{
				DEFINE_BLOCK(I+1, COLORINDEX_NORMAL);
			}
		}
cmpnextchar:
		i = 0;
	}
	
Out: //当遇到单行注释时会直接跳到这儿
	if (dwCookie & COOKIE_TAG) {
		if (nIdentBegin >= 0)
		{
			CString strtmp = strLine.Mid(nIdentBegin, I - nIdentBegin); 
			if (IsSynWord(m_strArrayKeyWords, strtmp))
			{
				if( (nIdentBegin > 0) && (strLine[nIdentBegin-1]=='/') ) {
					DEFINE_BLOCK(nIdentBegin-1, COLORINDEX_SYNTAX);
				}
				else {
					DEFINE_BLOCK(nIdentBegin, COLORINDEX_SYNTAX);
				}
				if(strtmp.CompareNoCase(_T("script"))==0) {
					if( nIdentBegin > 0 && strLine[nIdentBegin-1] == '<') {
						dwCookie |= COOKIE_SCRIPT;
					}
					else if( nIdentBegin > 1 && strLine[nIdentBegin-1] == '/' && strLine[nIdentBegin-12] == '<') {
						dwCookie &= ~COOKIE_SCRIPT;
						DEFINE_BLOCK(nIdentBegin-2, COLORINDEX_NORMAL);
					}
				}
			}
			else if (IsNumber(strtmp))
			{
				DEFINE_BLOCK(nIdentBegin, COLORINDEX_NUMBER);
			}
		}
	}

	return dwCookie;
}


void LoadParseRule(int nlanguage) 
{
	g_strInString = _T("");
	g_nstrInStringCount = 0;
	g_nBeginCommentExStrCount = 0; 
	g_nEndCommentExStrCount = 0; 
	g_nLineCommentStrCount = 0; 
	g_nBeginStringStrCount = 0; 
	g_nEndStringStrCount = 0; 
	g_nBeginCharStrCount = 0; 
	g_nEndCharStrCount = 0; 
	g_bControl = FALSE;
	g_bCase = FALSE;
/////////////////////////////////////////////////////////////////////////////
	if(g_BeginCommentExStr[0]!= NULL) {delete g_BeginCommentExStr[0]; g_BeginCommentExStr[0] = NULL;}
	if(g_BeginCommentExStr[1]!= NULL) {delete g_BeginCommentExStr[1]; g_BeginCommentExStr[1] = NULL;}
/////////////////////////////////////////////////////////////////////////////
	if(g_EndCommentExStr[0]!= NULL) {delete g_EndCommentExStr[0]; g_EndCommentExStr[0] = NULL;}
	if(g_EndCommentExStr[1]!= NULL) {delete g_EndCommentExStr[1]; g_EndCommentExStr[1] = NULL;}
/////////////////////////////////////////////////////////////////////////////
	if(g_LineCommentStr[0]!= NULL) {delete g_LineCommentStr[0]; g_LineCommentStr[0] = NULL;}
	if(g_LineCommentStr[1]!= NULL) {delete g_LineCommentStr[1]; g_LineCommentStr[1] = NULL;}
/////////////////////////////////////////////////////////////////////////////
	if(g_BeginStringStr[0]!= NULL) {delete g_BeginStringStr[0]; g_BeginStringStr[0] = NULL;}
/////////////////////////////////////////////////////////////////////////////
	if(g_EndStringStr[0]!= NULL) {delete g_EndStringStr[0]; g_EndStringStr[0] = NULL;}
/////////////////////////////////////////////////////////////////////////////
	if(g_BeginCharStr[0]!= NULL) {delete g_BeginCharStr[0]; g_BeginCharStr[0] = NULL;}
/////////////////////////////////////////////////////////////////////////////
	if(g_EndCharStr[0]!= NULL) {delete g_EndCharStr[0]; g_EndCharStr[0] = NULL;}

	CString strInString = _T("");
	CString BeginCommentExStr0 = _T("");
	CString EndCommentExStr0 = _T("");
	CString BeginCommentExStr1 = _T("");
	CString EndCommentExStr1 = _T("");
	CString LineCommentStr0 = _T("");
	CString LineCommentStr1 = _T("");
	CString BeginStringStr0 = _T("");
	CString EndStringStr0 = _T("");
	CString BeginCharStr0 = _T("");
	CString EndCharStr0 = _T("");

	switch ( nlanguage ) {
	case _ADA:
		LineCommentStr0=_T("--");
		break;
	case _AML:
		strInString = _T("&");
		LineCommentStr0=_T("/*");
		BeginStringStr0=_T("%");
		EndStringStr0=_T("%");
		BeginCharStr0=_T("[");
		EndCharStr0=_T("]");
		break;
	case _ASM:
		LineCommentStr0=_T(";");
		BeginCommentExStr0=_T("[");
		EndCommentExStr0=_T("]");
		break;
	case _BASIC:
		strInString = _T("&#");
		LineCommentStr0=_T("\'");
		LineCommentStr1=_T("rem");
		BeginStringStr0=_T("\"");
		EndStringStr0=_T("\"");
		break;
	case _BATCH:
		LineCommentStr0=_T("rem");
		break;
	case _BLUE:
		LineCommentStr0=_T("--");
		LineCommentStr1=_T("==");
		BeginCommentExStr0=_T("\"");
		EndCommentExStr0=_T("\"");
		break;
	case _CLIPPER:
		strInString = _T(".");
		LineCommentStr0=_T("&&");
		LineCommentStr1=_T("//");
		BeginCommentExStr0=_T("/*");
		EndCommentExStr0=_T("*/");
		break;
	case _COBOL:
		strInString = _T("-");
		LineCommentStr0=_T("*>");
		break;
	case _CPP:
		strInString = _T("#"); 
		BeginCommentExStr0=_T("/*"); 
		EndCommentExStr0=_T("*/");
		LineCommentStr0=_T("//");
		BeginStringStr0=_T("\"");
		EndStringStr0=_T("\"");
		BeginCharStr0=_T("\'");
		EndCharStr0=_T("\'");
		g_bControl = TRUE;
		g_bCase = TRUE;
		break;
	case _CSHAPE:
		strInString = _T("#");
		BeginCommentExStr0=_T("/*");
		EndCommentExStr0=_T("*/");
		LineCommentStr0=_T("//");
		BeginStringStr0=_T("\"");
		EndStringStr0=_T("\"");
		BeginCharStr0=_T("\'");
		EndCharStr0=_T("\'");
		g_bControl = TRUE;
		g_bCase = TRUE;
		break;
	case _DATAFLEX:
		strInString = _T("$");
		LineCommentStr0=_T("//");
		break;
	case _EIFFEL:
		LineCommentStr0=_T("--");
		break;
	case _FORTRAN:
		strInString = _T(".");
		LineCommentStr0=_T("!");
		break;
	case _HLBL:
		strInString = _T("#");
		BeginCommentExStr0=_T("{");
		EndCommentExStr0=_T("}");
		BeginCommentExStr1=_T("/*");
		EndCommentExStr1=_T("*/");
		BeginStringStr0=_T("[");
		EndStringStr0=_T("]");
		BeginCharStr0=_T("$$");
		EndCharStr0=_T("$$");
		break;
//*
	case _HTML:
		strInString = _T("-#");
		BeginCommentExStr0=_T("<!--");
		EndCommentExStr0=_T("-->");
		BeginStringStr0=_T("\"");
		EndStringStr0=_T("\"");
		BeginCharStr0=_T("'");
		EndCharStr0=_T("'");
		break;
//*/
	case _INI:
		LineCommentStr0=_T(";");
		BeginStringStr0=_T("[");
		EndStringStr0=_T("]");
		break;
	case _JAVA:
		LineCommentStr0=_T("//");
		BeginCommentExStr0=_T("/*");
		EndCommentExStr0=_T("*/");
		break;
	case _JSP:
		LineCommentStr0=_T("//");
		BeginCommentExStr0=_T("/*");
		EndCommentExStr0=_T("*/");
		break;
	case _LIMBO:
		LineCommentStr0=_T("#");
		break;
	case _LISP:
		LineCommentStr0=_T(";");
		break;
	case _MODULA2:
		BeginCommentExStr0=_T("(*");
		EndCommentExStr0=_T("*)");
		break;
	case _OBJECTPASCAL:
		strInString = _T("$");
		LineCommentStr0=_T("//");
		BeginCommentExStr0=_T("{");
		EndCommentExStr0=_T("}");
		BeginCommentExStr1=_T("(*");
		EndCommentExStr1=_T("*)");
		BeginStringStr0=_T("\'");
		EndStringStr0=_T("\'");
		BeginCharStr0=_T("[");
		EndCharStr0=_T("]");
		break;
	case _PARADOX:
		LineCommentStr0=_T(";");
		BeginCommentExStr0=_T("{");
		EndCommentExStr0=_T("}");
		break;
	case _PERL:
		LineCommentStr0=_T("//");
		LineCommentStr1=_T("#");
		BeginCommentExStr0=_T("/*");
		EndCommentExStr0=_T("*/");
		break;
	case _PHP:
		LineCommentStr0=_T("//");
		LineCommentStr1=_T("#");
		BeginCommentExStr0=_T("/*");
		EndCommentExStr0=_T("*/");
		break;
	case _PL1:
		strInString = _T("-");
		break;
	case _PROGRESS:
		strInString = _T("-");
		BeginCommentExStr0=_T("/*");
		EndCommentExStr0=_T("*/");
		BeginCommentExStr1=_T("{");
		EndCommentExStr1=_T("}");
		break;
	case _PYTHON:
		LineCommentStr0=_T("#");
		BeginStringStr0=_T("\"");
		EndStringStr0=_T("\"");
		break;
	case _REBOL:
		strInString = _T("!?-");
		LineCommentStr0=_T(";--");
		BeginStringStr0=_T("{");
		EndStringStr0=_T("}");
		BeginCharStr0=_T("<");
		EndCharStr0=_T(">");
		break;
	case _REXX:
		BeginCommentExStr0=_T("/*");
		EndCommentExStr0=_T("*/");
		break;
	case _RUBY:
		LineCommentStr0=_T("#");
		BeginCommentExStr0=_T("=begin");
		EndCommentExStr0=_T("=end");
		break;
	case _SMALLTALK:
		LineCommentStr0=_T("!");
		BeginCommentExStr0=_T("\"");
		EndCommentExStr0=_T("\"");
		break;
	case _SQL:
		LineCommentStr0=_T("--");
		BeginCommentExStr0=_T("/*");
		EndCommentExStr0=_T("*/");
		break;
	case _VHDL:
		LineCommentStr0=_T("--");
		break;
	case _XML:
		BeginCommentExStr0=_T("<!--");
		EndCommentExStr0=_T("-->");
		break;
	default: //is NONE
		return;
	}

	CString strLanguagePos = _T("icrEdit__ParseRule");
	strLanguagePos += GetLanguageString(nlanguage);

	CString strtmp;

	CWinApp *app = AfxGetApp();
	g_bCase = app->GetProfileInt( strLanguagePos, _T("g_bCase"), g_bCase);

	// 一个单词包含的字符常量
	strtmp = app->GetProfileString( strLanguagePos, _T("strInString"), strInString);
	strtmp.TrimLeft();strtmp.TrimRight();strtmp.TrimLeft(0x9);strtmp.TrimRight(0x9);
	if(!strtmp.IsEmpty()) { 
		g_strInString = strtmp;
		g_nstrInStringCount = g_strInString.GetLength(); 
	}
	//一个单词包含的字符常量

	// 开始多行注释的字符串
	strtmp = app->GetProfileString( strLanguagePos, _T("BeginCommentExStr0"), BeginCommentExStr0);
	strtmp.TrimLeft();strtmp.TrimRight();strtmp.TrimLeft(0x9);strtmp.TrimRight(0x9);
	if(!strtmp.IsEmpty()) { 
		g_BeginCommentExStr[0] = new TCHAR[strtmp.GetLength()+1];
		strcpy(g_BeginCommentExStr[0], strtmp);
		g_nBeginCommentExStrCount = 1;
	}

	strtmp = app->GetProfileString( strLanguagePos, _T("BeginCommentExStr1"), BeginCommentExStr1);
	strtmp.TrimLeft();strtmp.TrimRight();strtmp.TrimLeft(0x9);strtmp.TrimRight(0x9);
	if(!strtmp.IsEmpty()) { 
		g_BeginCommentExStr[1] = new TCHAR[strtmp.GetLength()+1];
		strcpy(g_BeginCommentExStr[1], strtmp);
		g_nBeginCommentExStrCount = 2;
	}
	//开始多行注释的字符串

	//结束多行注释的字符串
	strtmp = app->GetProfileString( strLanguagePos, _T("EndCommentExStr0"), EndCommentExStr0);
	strtmp.TrimLeft();strtmp.TrimRight();strtmp.TrimLeft(0x9);strtmp.TrimRight(0x9);
	if(!strtmp.IsEmpty()) { 
		g_EndCommentExStr[0] = new TCHAR[strtmp.GetLength()+1];
		strcpy(g_EndCommentExStr[0], strtmp);
		g_nEndCommentExStrCount = 1;
	}

	strtmp = app->GetProfileString( strLanguagePos, _T("EndCommentExStr1"), EndCommentExStr1);
	strtmp.TrimLeft();strtmp.TrimRight();strtmp.TrimLeft(0x9);strtmp.TrimRight(0x9);
	if(!strtmp.IsEmpty()) { 
		g_EndCommentExStr[1] = new TCHAR[strtmp.GetLength()+1];
		strcpy(g_EndCommentExStr[1], strtmp);
		g_nEndCommentExStrCount = 2;
	}
	//结束多行注释的字符串

	//开始单行注释的字符串
	strtmp = app->GetProfileString( strLanguagePos, _T("LineCommentStr0"), LineCommentStr0);
	strtmp.TrimLeft();strtmp.TrimRight();strtmp.TrimLeft(0x9);strtmp.TrimRight(0x9);
	if(!strtmp.IsEmpty()) { 
		g_LineCommentStr[0] = new TCHAR[strtmp.GetLength()+1];
		strcpy(g_LineCommentStr[0], strtmp);
		g_nLineCommentStrCount = 1;
	}

	strtmp = app->GetProfileString( strLanguagePos, _T("LineCommentStr1"), LineCommentStr1);
	strtmp.TrimLeft();strtmp.TrimRight();strtmp.TrimLeft(0x9);strtmp.TrimRight(0x9);
	if(!strtmp.IsEmpty()) { 
		g_LineCommentStr[1] = new TCHAR[strtmp.GetLength()+1];
		strcpy(g_LineCommentStr[1], strtmp);
		g_nLineCommentStrCount = 2;
	}
	//开始单行注释的字符串

	//开始字符串变量的字符串
	strtmp = app->GetProfileString( strLanguagePos, _T("BeginStringStr0"), BeginStringStr0);
	strtmp.TrimLeft();strtmp.TrimRight();strtmp.TrimLeft(0x9);strtmp.TrimRight(0x9);
	if(!strtmp.IsEmpty()) { 
		g_BeginStringStr[0] = new TCHAR[strtmp.GetLength()+1];
		strcpy(g_BeginStringStr[0], strtmp);
		g_nBeginStringStrCount = 1;
	}
	//开始字符串变量的字符串

	//结束字符串变量的字符串
	strtmp = app->GetProfileString( strLanguagePos, _T("EndStringStr0"), EndStringStr0);
	strtmp.TrimLeft();strtmp.TrimRight();strtmp.TrimLeft(0x9);strtmp.TrimRight(0x9);
	if(!strtmp.IsEmpty()) { 
		g_EndStringStr[0] = new TCHAR[strtmp.GetLength()+1];
		strcpy(g_EndStringStr[0], strtmp);
		g_nEndStringStrCount = 1;
	}
	//结束字符串变量的字符串

	//开始字符型常量的字符串
	strtmp = app->GetProfileString( strLanguagePos, _T("BeginCharStr0"), BeginCharStr0);
	strtmp.TrimLeft();strtmp.TrimRight();strtmp.TrimLeft(0x9);strtmp.TrimRight(0x9);
	if(!strtmp.IsEmpty()) { 
		g_BeginCharStr[0] = new TCHAR[strtmp.GetLength()+1];
		strcpy(g_BeginCharStr[0], strtmp);
		g_nBeginCharStrCount = 1;
	}
	//开始字符型常量的字符串

	//结束字符型常量的字符串
	strtmp = app->GetProfileString( strLanguagePos, _T("EndCharStr0"), EndCharStr0);
	strtmp.TrimLeft();strtmp.TrimRight();strtmp.TrimLeft(0x9);strtmp.TrimRight(0x9);
	if(!strtmp.IsEmpty()) { 
		g_EndCharStr[0] = new TCHAR[strtmp.GetLength()+1];
		strcpy(g_EndCharStr[0], strtmp);
		g_nEndCharStrCount = 1;
	}
	//结束字符型常量的字符串
}


CString GetLanguageString(int nlanguage)
{
	switch ( nlanguage ) {
	case _ADA:
		return _T("_ADA");
	case _AML:
		return _T("_AML");
	case _ASM:
		return _T("_ASM");
	case _BASIC:
		return _T("_BASIC");
	case _BATCH:
		return _T("_BATCH");
	case _BLUE:
		return _T("_BLUE");
	case _CLIPPER:
		return _T("_CLIPPER");
	case _COBOL:
		return _T("_COBOL");
	case _CPP:
		return _T("_CPP");
	case _CSHAPE:
		return _T("_CSHAPE");
	case _DATAFLEX:
		return _T("_DATAFLEX");
	case _EIFFEL:
		return _T("_EIFFEL");
	case _FORTRAN:
		return _T("_FORTRAN");
	case _HLBL:
		return _T("_HLBL");
//*
	case _HTML:
		return _T("_HTML");
//*/
	case _INI:
		return _T("_INI");
	case _JAVA:
		return _T("_JAVA");
	case _JSP:
		return _T("_JSP");
	case _LIMBO:
		return _T("_LIMBO");
	case _LISP:
		return _T("_LISP");
	case _MODULA2:
		return _T("_MODULA2");
	case _OBJECTPASCAL:
		return _T("_OBJECTPASCAL");
	case _PARADOX:
		return _T("_PARADOX");
	case _PERL:
		return _T("_PERL");
	case _PHP:
		return _T("_PHP");
	case _PL1:
		return _T("_PL1");
	case _PROGRESS:
		return _T("_PROGRESS");
	case _PYTHON:
		return _T("_PYTHON");
	case _REBOL:
		return _T("_REBOL");
	case _REXX:
		return _T("_REXX");
	case _RUBY:
		return _T("_RUBY");
	case _SMALLTALK:
		return _T("_SMALLTALK");
	case _SQL:
		return _T("_SQL");
	case _VHDL:
		return _T("_VHDL");
	case _XML:
		return _T("_XML");
	default: //is NONE
		return _T("Plain text");
	}
}

void DeleteAllocString()
{
	//删除分配的语法解析规则字符串指针以防止内存泄漏
	if(g_BeginCommentExStr[0]!= NULL) {delete g_BeginCommentExStr[0]; g_BeginCommentExStr[0] = NULL;}
	if(g_BeginCommentExStr[1]!= NULL) {delete g_BeginCommentExStr[1]; g_BeginCommentExStr[1] = NULL;}

	if(g_EndCommentExStr[0]!= NULL) {delete g_EndCommentExStr[0]; g_EndCommentExStr[0] = NULL;}
	if(g_EndCommentExStr[1]!= NULL) {delete g_EndCommentExStr[1]; g_EndCommentExStr[1] = NULL;}

	if(g_LineCommentStr[0]!= NULL) {delete g_LineCommentStr[0]; g_LineCommentStr[0] = NULL;}
	if(g_LineCommentStr[1]!= NULL) {delete g_LineCommentStr[1]; g_LineCommentStr[1] = NULL;}

	if(g_BeginStringStr[0]!= NULL) {delete g_BeginStringStr[0]; g_BeginStringStr[0] = NULL;}
	if(g_EndStringStr[0]!= NULL) {delete g_EndStringStr[0]; g_EndStringStr[0] = NULL;}

	if(g_BeginCharStr[0]!= NULL) {delete g_BeginCharStr[0]; g_BeginCharStr[0] = NULL;}
	if(g_EndCharStr[0]!= NULL) {delete g_EndCharStr[0]; g_EndCharStr[0] = NULL;}
}


CString GetLineCommentString(int nlanguage)
{
	CString LineCommentStr0 = _T("");

	switch ( nlanguage ) {
	case _ADA:
		LineCommentStr0=_T("--");
		break;
	case _AML:
		LineCommentStr0=_T("/*");
		break;
	case _ASM:
		LineCommentStr0=_T(";");
		break;
	case _BASIC:
		LineCommentStr0=_T("\'");
		break;
	case _BATCH:
		LineCommentStr0=_T("rem");
		break;
	case _BLUE:
		LineCommentStr0=_T("--");
		break;
	case _CLIPPER:
		LineCommentStr0=_T("&&");
		break;
	case _COBOL:
		LineCommentStr0=_T("*>");
		break;
	case _CPP:
		LineCommentStr0=_T("//");
		break;
	case _CSHAPE:
		LineCommentStr0=_T("//");
		break;
	case _DATAFLEX:
		LineCommentStr0=_T("//");
		break;
	case _EIFFEL:
		LineCommentStr0=_T("--");
		break;
	case _FORTRAN:
		LineCommentStr0=_T("!");
		break;
	case _HLBL:
		break;
//*
	case _HTML:
		break;
//*/
	case _INI:
		LineCommentStr0=_T(";");
		break;
	case _JAVA:
		LineCommentStr0=_T("//");
		break;
	case _JSP:
		LineCommentStr0=_T("//");
		break;
	case _LIMBO:
		LineCommentStr0=_T("#");
		break;
	case _LISP:
		LineCommentStr0=_T(";");
		break;
	case _MODULA2:
		break;
	case _OBJECTPASCAL:
		LineCommentStr0=_T("//");
		break;
	case _PARADOX:
		LineCommentStr0=_T(";");
		break;
	case _PERL:
		LineCommentStr0=_T("//");
		break;
	case _PHP:
		LineCommentStr0=_T("//");
		break;
	case _PL1:
		break;
	case _PROGRESS:
		break;
	case _PYTHON:
		LineCommentStr0=_T("#");
		break;
	case _REBOL:
		LineCommentStr0=_T(";--");
		break;
	case _REXX:
		break;
	case _RUBY:
		LineCommentStr0=_T("#");
		break;
	case _SMALLTALK:
		LineCommentStr0=_T("!");
		break;
	case _SQL:
		LineCommentStr0=_T("--");
		break;
	case _VHDL:
		LineCommentStr0=_T("--");
		break;
	case _XML:
		break;
	default: //is NONE
		return LineCommentStr0;
	}

	CString strLanguagePos = _T("icrEdit__ParseRule");
	strLanguagePos += GetLanguageString(nlanguage);
	CWinApp *app = AfxGetApp();
	LineCommentStr0 = app->GetProfileString( strLanguagePos, _T("LineCommentStr0"), LineCommentStr0);
	LineCommentStr0.TrimLeft();
	LineCommentStr0.TrimRight();
	LineCommentStr0.TrimLeft(0x9);
	LineCommentStr0.TrimRight(0x9);
	if(!LineCommentStr0.IsEmpty()) 
		return LineCommentStr0;

	LineCommentStr0 = app->GetProfileString( strLanguagePos, _T("LineCommentStr1"), LineCommentStr0);
	LineCommentStr0.TrimLeft();
	LineCommentStr0.TrimRight();
	LineCommentStr0.TrimLeft(0x9);
	LineCommentStr0.TrimRight(0x9);
	return LineCommentStr0;
}

//根据显示给用户看的字符串取出语言
int GetLanguageByStringShow(CString strlanguage)
{
	if(strlanguage==_T("Ada"))
		return _ADA;
	if(strlanguage==_T("AML"))
		return _AML;
	if(strlanguage==_T("ASM"))
		return _ASM;
	if(strlanguage==_T("Basic"))
		return _BASIC;
	if(strlanguage==_T("BAT"))
		return _BATCH;
	if(strlanguage==_T("Blue"))
		return _BLUE;
	if(strlanguage==_T("Clipper"))
		return _CLIPPER;
	if(strlanguage==_T("COBOL"))
		return _COBOL;
	if(strlanguage==_T("C++"))
		return _CPP;
	if(strlanguage==_T("C#"))
		return _CSHAPE;
	if(strlanguage==_T("Dataflex"))
		return _DATAFLEX;
	if(strlanguage==_T("Eiffel"))
		return _EIFFEL;
	if(strlanguage==_T("FORTRAN"))
		return _FORTRAN;
	if(strlanguage==_T("HLBL"))
		return _HLBL;
//*
	if(strlanguage==_T("HTML"))
		return _HTML;
//*/
	if(strlanguage==_T("INI"))
		return _INI;
	if(strlanguage==_T("Java"))
		return _JAVA;
	if(strlanguage==_T("JSP"))
		return _JSP;
	if(strlanguage==_T("LIMBO"))
		return _LIMBO;
	if(strlanguage==_T("LISP"))
		return _LISP;
	if(strlanguage==_T("Object Pascal"))
		return _OBJECTPASCAL;
	if(strlanguage==_T("Modula2"))
		return _MODULA2;
	if(strlanguage==_T("Paradox"))
		return _PARADOX;
	if(strlanguage==_T("Perl"))
		return _PERL;
	if(strlanguage==_T("PHP"))
		return _PHP;
	if(strlanguage==_T("PL1"))
		return _PL1;
	if(strlanguage==_T("Progress"))
		return _PROGRESS;
	if(strlanguage==_T("Python"))
		return _PYTHON;
	if(strlanguage==_T("REBOL"))
		return _REBOL;
	if(strlanguage==_T("REXX"))
		return _REXX;
	if(strlanguage==_T("RUBY"))
		return _RUBY;
	if(strlanguage==_T("Smalltalk"))
		return _SMALLTALK;
	if(strlanguage==_T("SQL"))
		return _SQL;
	if(strlanguage==_T("VHDL"))
		return _VHDL;
	if(strlanguage==_T("XML"))
		return _XML;
	return NONE;
}

//根据语言返回给用户看的字符串
CString GetStringShowByLanguage(int nlanguage)
{
	switch(nlanguage) {
	case _ADA:
		return _T("Ada");
	case _AML:
		return _T("AML");
	case _ASM:
		return _T("ASM");
	case _BASIC:
		return _T("Basic");
	case _BATCH:
		return _T("BAT");
	case _BLUE:
		return _T("Blue");
	case _CLIPPER:
		return _T("Clipper");
	case _COBOL:
		return _T("COBOL");
	case _CPP:
		return _T("C++");
	case _CSHAPE:
		return _T("C#");
	case _DATAFLEX:
		return _T("Dataflex");
	case _EIFFEL:
		return _T("Eiffel");
	case _FORTRAN:
		return _T("FORTRAN");
	case _HLBL:
		return _T("HLBL");
//*
	case _HTML:
		return _T("HTML");
//*/
	case _INI:
		return _T("INI");
	case _JAVA:
		return _T("Java");
	case _JSP:
		return _T("JSP");
	case _LIMBO:
		return _T("LIMBO");
	case _LISP:
		return _T("LISP");
	case _OBJECTPASCAL:
		return _T("Object Pascal");
	case _MODULA2:
		return _T("Modula2");
	case _PARADOX:
		return _T("Paradox");
	case _PERL:
		return _T("Perl");
	case _PHP:
		return _T("PHP");
	case _PL1:
		return _T("PL1");
	case _PROGRESS:
		return _T("Progress");
	case _PYTHON:
		return _T("Python");
	case _REBOL:
		return _T("REBOL");
	case _REXX:
		return _T("REXX");
	case _RUBY:
		return _T("RUBY");
	case _SMALLTALK:
		return _T("Smalltalk");
	case _SQL:
		return _T("SQL");
	case _VHDL:
		return _T("VHDL");
	case _XML:
		return _T("XML");
	default:
		return _T("Plain text");
	}
}
