#ifndef CROPCPP_IDPQUERYOPERATOR_H
#define CROPCPP_IDPQUERYOPERATOR_H


enum IdpQueryOperator
{   NestedLoop = 0,
    Hash = 1
};

double opCost(IdpQueryOperator op);

#endif //CROPCPP_IDPQUERYOPERATOR_H
