#include "Joint.hpp"

Joint::Joint(unsigned int TypeIn, arma::vec piIn, arma::vec pjIn, arma::vec qiIn,
            arma::vec qjIn, Body *i_In, Body *j_In) :
    pi(3, arma::fill::zeros),
    pj(3, arma::fill::zeros),
    qi(3, arma::fill::zeros),
    qj(3, arma::fill::zeros),
    Cq(3, 12, arma::fill::zeros),
    GAMMA(3, arma::fill::zeros),
    CONSTRAINT(3, arma::fill::zeros),
    TBI_i(3, 3, arma::fill::eye),
    TBI_j(3, 3, arma::fill::eye),
    Pi(3, arma::fill::zeros),
    Pj(3, arma::fill::zeros),
    Qi(3, arma::fill::zeros),
    Qj(3, arma::fill::zeros),
    wi(3, arma::fill::zeros),
    wj(3, arma::fill::zeros),
    Si(3, arma::fill::zeros),
    Sj(3, arma::fill::zeros) {
        Type = TypeIn;
        body_i_ptr = i_In;
        body_j_ptr = j_In;
        pi = piIn;
        pj = pjIn;
        qi = qiIn;
        qj = qjIn;

        update();        
}

void Joint::update() {
    TBI_i = body_i_ptr->get_TBI();
    TBI_j = body_j_ptr->get_TBI();
    wi = body_i_ptr->get_ANG_VEL();
    wj = body_j_ptr->get_ANG_VEL();
    Pi = trans(TBI_i) * pi;
    Pj = trans(TBI_j) * pj;
    Qi = trans(TBI_i) * qi;
    Qj = trans(TBI_j) * qj;
    Si = body_i_ptr->get_POSITION();
    Sj = body_j_ptr->get_POSITION();

    Build_C();
    Build_Cq();
    Build_GAMMA();
}

void Joint::Build_C() {
    CONSTRAINT = Si + Pi - Sj - Pj;
}

void Joint::Build_Cq() {
    arma::mat temp_matrix_1(3, 3, arma::fill::eye);
    arma::mat temp_matrix_2(3, 3, arma::fill::zeros);
    arma::mat temp_matrix_3(3, 3, arma::fill::eye);
    arma::mat temp_matrix_4(3, 3, arma::fill::zeros);

    temp_matrix_2 = -skew_sym(Pi) * trans(TBI_i);
    temp_matrix_4 = skew_sym(Pj) * trans(TBI_j);

    Cq = join_rows(temp_matrix_1, join_rows(temp_matrix_2, join_rows(-temp_matrix_3, temp_matrix_4)));
}

void Joint::Build_GAMMA() {
    arma::mat33 Skew_Omega_i = skew_sym(trans(TBI_i) * wi);
    arma::mat33 Skew_Omega_j = skew_sym(trans(TBI_j) * wj);

    GAMMA = -trans(TBI_i) * Skew_Omega_i * Skew_Omega_i * pi + trans(TBI_j) * Skew_Omega_j * Skew_Omega_j * pj;
}

arma::mat Joint::get_Cq() { return Cq; }
arma::vec Joint::get_GAMMA() { return GAMMA; }










