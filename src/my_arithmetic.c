/*
 *	程序名：my_arithmetic.c
 *	作者：陈源源
 *	日期：2013-02-10
 *	功能：包含算术运算操作
 */

#include <string.h>
#include <errno.h>
#include "my_arithmetic.h"
#include "my_log.h"

/*
 *	功能：比较有理数的大小
 *	参数：
 *		a，b：要比较的有理数
 *		cmp_res：比较结果：
 *			>0：a>b
 *			0：a=b
 *			<0：a<b
 *	返回值：
 *		MY_SUCC：成功
 *		MY_ERROR：出错
 */
int my_rats_cmp(my_rat* a,my_rat* b,int32_t* cmp_res)
{
	size_t a_digit_num,b_digit_num;
	my_node *p,*q;

	//检查参数
	if(!a)
	{
		my_log("a is NULL");
		return MY_ERROR;	
	}
	if(!MY_RAT_HAS_INITED(a))
	{
		my_log("a is uninitialized");
		return MY_ERROR;	
	}
	if(!b)
	{
		my_log("b is NULL");
		return MY_ERROR;	
	}
	if(!MY_RAT_HAS_INITED(b))
	{
		my_log("b is uninitialized");
		return MY_ERROR;	
	}

	if(!cmp_res)
	{
		my_log("cmp_res is NULL");
		return MY_ERROR;	
	}
	my_rat_strip_leading_zero(a);
	my_rat_strip_ending_zero(a);
	my_rat_strip_leading_zero(b);
	my_rat_strip_ending_zero(b);

	if(a->msn->data==0) //0
		a->sign=1;
	if(b->msn->data==0) //0
		b->sign=1;
	//符号不等
	if(a->sign > b->sign)
	{
		*cmp_res=1;
		return MY_SUCC;
	}

	if(a->sign < b->sign)
	{
		*cmp_res=-1;
		return MY_SUCC;
	}

	p=a->msn;
	q=b->msn;
	//FIXME:可能溢出
	a_digit_num=MY_RAT_DIGIT_NUM(p)+((MY_RAT_USED_NODE_NUM(a)-1)<<2)+a->power;
	b_digit_num=MY_RAT_DIGIT_NUM(q)+((MY_RAT_USED_NODE_NUM(b)-1)<<2)+b->power;

	if(a_digit_num != b_digit_num)
	{
		if(a_digit_num>b_digit_num)
			*cmp_res=1;
		else
			*cmp_res=-1;
	}
	else
	{
		*cmp_res=0;
		size_t min_node_num=MY_MIX(MY_RAT_USED_NODE_NUM(a),MY_RAT_USED_NODE_NUM(b));
		while(min_node_num)
		{
			if(p->data != q->data)
			{
				*cmp_res=p->data-q->data;
				break;
			}
			p=p->prev;
			q=q->prev;
			min_node_num--;
		}

		if(*cmp_res==0)
		{
			if(MY_RAT_USED_NODE_NUM(a)>MY_RAT_USED_NODE_NUM(b))
				*cmp_res=1;
			else if(MY_RAT_USED_NODE_NUM(a)<MY_RAT_USED_NODE_NUM(b))
				*cmp_res=-1;
		}
	}
	if(a->sign==-1) //复数
		*cmp_res=-(*cmp_res);
	return MY_SUCC;
}

/*
 *	功能：比较有理数的绝对值大小
 *	参数：
 *		a，b：要比较的有理数
 *		cmp_res：比较结果：
 *			>0：|a|>|b|
 *			0：|a|=|b|
 *			<0：|a|<|b|
 *	返回值：
 *		MY_SUCC：成功
 *		MY_ERROR：出错
 */
int my_rats_cmp_abs(my_rat* a,my_rat* b,int32_t* cmp_res)
{
	int sign_a,sign_b;
	int res;

	//检查参数
	if(!a)
	{
		my_log("a is NULL");
		return MY_ERROR;	
	}
	if(!MY_RAT_HAS_INITED(a))
	{
		my_log("a is uninitialized");
		return MY_ERROR;	
	}
	if(!b)
	{
		my_log("b is NULL");
		return MY_ERROR;	
	}
	if(!MY_RAT_HAS_INITED(b))
	{
		my_log("b is uninitialized");
		return MY_ERROR;	
	}

	if(!cmp_res)
	{
		my_log("cmp_res is NULL");
		return MY_ERROR;	
	}
		
	//保存正负号
	sign_a=a->sign;
	sign_b=b->sign;
	//设置为正数
	a->sign=1;
	b->sign=1;
	res=my_rats_cmp(a,b,cmp_res);
	//恢复
	a->sign=sign_a;
	b->sign=sign_b;
	return res;
}




/*
 *	功能：有理数乘以小整数
 *	参数:
 *		a：有理数
 *		b：必须在[-9999,9999]区间
 *		type：结果存放方式，取值以下：
 *			MY_NEW_RES：积作为新的有理数返回
 *			MY_ARG_RES：积放在a
 *	返回值：
 *		非NULL：积
 *		NULL：出错
 */
my_rat* my_rat_multiply_small_int(my_rat* a,int32_t b,my_result_saving_type type)
{
	my_rat* c;
	my_node *p,*q;
	size_t node_num;
	int32_t tmp,carry;

	//验证参数
	if(b>9999 || b < -9999)
	{
		my_log("b is out of [-9999,9999]");
		return NULL;	
	}

	if(a)
	{
		if(!MY_RAT_HAS_INITED(a))
		{
			my_log("a is uninitialized");
			return NULL;	
		}
		c=a;
	}
	else
	{
		//分配空间
		c=(my_rat *)calloc(1,sizeof(*c));
		if(!c)
		{
			my_log("calloc failed:%s",strerror(errno));
			return NULL;			
		}
	}

	//去除0
	my_rat_strip_leading_zero(a);
	my_rat_strip_ending_zero(a);

	//乘法可能导致溢出
	if(MY_RAT_FREE_NODE_NUM(c)==0)
	{
		if(my_rat_add_node(c,1) !=MY_SUCC)
		{
			my_log("my_rat_add_node failed");
			if(!a)
				my_rat_free(c);
			return NULL;
		}
	}

	if(b<0)
	{
		b=-b;
		c->sign=-(c->sign);
	}

	carry=0;
	p=a->lsn;
	q=c->lsn;
	node_num=MY_RAT_USED_NODE_NUM(a);
	c->used_node_num=node_num;
	while(node_num)
	{
		/*
		__asm__("imull %3, %%eax  \n\t xorl %%edx,%%edx \n\t movl $10000,%%ecx \n\t idivl %%ecx" 
				: "=a" (tmp2), "=d" (q->data) 
				:"a"(b) ,"g"(p->data):"%ecx","cc");
*/
		tmp=p->data*b;
		q->data=tmp%10000+carry;
		carry=tmp/10000;
		p=p->next;
		q=q->next;
		node_num--;
	}
	if(carry)
	{
		c->msn=p;
		q->data=carry;
		c->used_node_num++;
	}
	else
		c->msn=q->prev;
	return c;
}









/*
 *	功能：有理数加法
 *	参数：
 *		a,b：要相加的有理数
 *		type：结果存放方式，取值以下：
 *			MY_NEW_RES：和作为新的有理数返回
 *			MY_ARG_RES：和放在a
 *	返回值：
 *		非NULL：和
 *		NULL：出错
 */
/*
my_rat* my_rats_add(my_rat* a,my_rat* b,my_result_saving_type type)
{
	ln c;
	int carry;
	cell x,y,z;
	int cmp_res;

	//验证参数
	if(!a)
	{
		my_log("a is NULL");
		return MY_ERROR;	
	}
	if(!MY_RAT_HAS_INITED(a))
	{
		my_log("a is uninitialized");
		return NULL;	
	}
	if(!b)
	{
		my_log("b is NULL");
		return MY_ERROR;	
	}
	if(!MY_RAT_HAS_INITED(b))
	{
		my_log("b is uninitialized");
		return NULL;	
	}

	//去除前置0
	my_rat_strip_leading_zero(a);
	my_rat_strip_leading_zero(b);

	//把指数调整为一致
	if(a->power > b->power)
		ln_adjustpower(a,b->power-a->power);
	if(a->power < b->power)
		ln_adjustpower(b,a->power-b->power);

	//根据存放方式设置c
	if(restype==firstln) 
		c=a;
	else
	{
		c=ln_creat(ln_cell_num(a));
		if(c==NULL)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_creat fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;	
		}
	}

	c->power=a->power;
	//a,b符号相同,加法
	if(a->sign==b->sign)	
	{
		//有可能会进位c多增加一个节点
		if(c->msd->hcell==c->lsd)
		{
			if(ln_addcell(c,1) !=LN_SUCC)
			{
				fprintf(stderr,"[%s %d] %s error,reason: ln_addcell fail\n",__FILE__,__LINE__,__FUNCTION__);
				return NULL;	
			}
		}
		//调整指数部分一致
		c->sign=a->sign;
		x=a->lsd;
		y=b->lsd;
		z=c->lsd;
		carry=0;
		while(1)
		{
			if(x)
				carry+=x->num;
			if(y)
				carry+=y->num;
			if(x==a->msd)
				x=NULL;
			if(y==b->msd)
				y=NULL;
			z->num=carry%UNIT;
			carry=carry/UNIT;
			if(x==NULL&&y==NULL&&carry==0)
			{
				c->msd=z;
				break;
			}
			if(x)
				x=x->hcell;
			if(y)
				y=y->hcell;
			z=z->hcell;
		}
		return c;
	}
	else //减法
	{
		cmp_res=ln_cmp_abs(a,b);
		if(cmp_res==2)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_cmp_abs fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;	
		}

		if(cmp_res==0) //绝对值相等 为0
		{
			ln_setval(c,0);
			return c;
		}
		//确定符号
		if(cmp_res==1)
		{
			x=a->lsd;
			y=b->lsd;
			c->sign=a->sign;
		}
		else	
		{
			x=b->lsd;
			y=a->lsd;
			c->sign=b->sign;
		}

		z=c->lsd;
		carry=0;
		while(1)
		{
			if(y)
				z->num=x->num-carry-y->num;
			else
				z->num=x->num-carry;
			if(z->num <0)
			{
				carry=1;
				z->num+=UNIT;	
			}
			else
				carry=0;
			if(y==a->msd || y==b->msd)
				y=NULL;
			if(x==a->msd || x==b->msd)
			{
				c->msd=z;
				break;
			}
			x=x->hcell;
			if(y)
				y=y->hcell;
			z=z->hcell;
		}	
		//消除前置0
		my_rat_strip_leading_zero(c);
		return c;
	}	
}
*/
