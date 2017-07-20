#include "poly.h"

edgestyle_t edgestyle_default;

windstate_t windstate_nonfilled = {
    0,
    0,
};

// -------------------- even/odd ----------------------

windstate_t evenodd_start(windcontext_t*context)
{
    return windstate_nonfilled;
}
windstate_t evenodd_add(windcontext_t*context, windstate_t left, edgestyle_t*edge, segment_dir_t dir, int master)
{
    assert(edge);
    left.is_filled ^= 1;
    return left;
}
edgestyle_t* evenodd_diff(windstate_t*left, windstate_t*right)
{
    if(left->is_filled==right->is_filled)
        return 0;
    else
        return &edgestyle_default;
}

windrule_t windrule_evenodd = {
    evenodd_start,
    evenodd_add,
    evenodd_diff,
};

// -------------------- circular ----------------------

windstate_t circular_start(windcontext_t*context)
{
    return windstate_nonfilled;
}

windstate_t circular_add(windcontext_t*context, windstate_t left, edgestyle_t*edge, segment_dir_t dir, int master)
{
    assert(edge);
    /* which one is + and which one - doesn't actually make any difference */
    if(dir == DIR_DOWN)
    left.wind_nr++;
    else
    left.wind_nr--;

    left.is_filled = left.wind_nr != 0;
    return left;
}

edgestyle_t* circular_diff(windstate_t*left, windstate_t*right)
{
    if(left->is_filled==right->is_filled)
        return 0;
    else
        return &edgestyle_default;
}

windrule_t windrule_circular = {
    circular_start,
    circular_add,
    circular_diff,
};

// -------------------- intersect ----------------------

windstate_t intersect_start(windcontext_t*context)
{
    return windstate_nonfilled;
}

windstate_t intersect_add(windcontext_t*context, windstate_t left, edgestyle_t*edge, segment_dir_t dir, int master)
{
    assert(master < context->num_polygons);

    left.wind_nr ^= 1<<master;
    left.is_filled = (left.wind_nr == (1<<context->num_polygons)-1);
    return left;
}

edgestyle_t* intersect_diff(windstate_t*left, windstate_t*right)
{
    if(left->is_filled==right->is_filled)
        return 0;
    else
        return &edgestyle_default;
}

windrule_t windrule_intersect = {
    intersect_start,
    intersect_add,
    intersect_diff,
};

// -------------------- union ----------------------

windstate_t union_start(windcontext_t*context)
{
    return windstate_nonfilled;
}

windstate_t union_add(windcontext_t*context, windstate_t left, edgestyle_t*edge, segment_dir_t dir, int master)
{
    assert(master<sizeof(left.wind_nr)*8); //up to 32/64 polygons max
    left.wind_nr ^= 1<<master;
    left.is_filled = (left.wind_nr!=0);
    return left;
}

edgestyle_t* union_diff(windstate_t*left, windstate_t*right)
{
    if(left->is_filled==right->is_filled)
        return 0;
    else
        return &edgestyle_default;
}

windrule_t windrule_union = {
    union_start,
    union_add,
    union_diff,
};


/*
 } else if (rule == WIND_NONZERO) {
     fill = wind!=0;
 } else if (rule == WIND_ODDEVEN) {
     fill = wind&1;
 } else { // rule == WIND_POSITIVE
     fill = wind>=1;
 }
 */
