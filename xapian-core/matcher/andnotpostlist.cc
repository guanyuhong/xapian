#include "andnotpostlist.h"

inline PostList *
AndNotPostList::advance_to_next_match(weight w_min, PostList *ret)
{
    if (ret) {
	delete l;
	l = ret;
    }    
    if (l->at_end()) {
	lhead = 0;
	return NULL;
    }
    lhead = l->get_docid();

    while (rhead <= lhead) {
	if (lhead == rhead) {
	    ret = l->next(w_min);
	    if (ret) {
		delete l;
		l = ret;
	    }
	    if (l->at_end()) {
		lhead = 0;
		return NULL;
	    }
	    lhead = l->get_docid();
	}		
	ret = r->skip_to(lhead, w_min);
	if (ret) {
	    delete r;
	    r = ret;
	}
	if (r->at_end()) {
	    ret = l;
	    l = NULL;
	    return ret;
	}
	rhead = r->get_docid();
    }
    return NULL;
}

AndNotPostList::AndNotPostList(PostList *left, PostList *right)
{    
    l = left;
    r = right;
    lhead = rhead = 0;
}

PostList *
AndNotPostList::next(weight w_min)
{
    return advance_to_next_match(w_min, l->next(w_min));
}

PostList *
AndNotPostList::skip_to(docid id, weight w_min)
{
    return advance_to_next_match(w_min, l->skip_to(id, w_min));
}
