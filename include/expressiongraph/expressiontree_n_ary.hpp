/*
 * expressiontree_n_ary.hpp
 *
 *  Created on: Oct 15, 2013, adapted May 2020
 *      Author: Erwin Aertbelien
 *
* expressiongraph library
* 
* Copyright 2014 Erwin Aertbelien - KU Leuven - Dep. of Mechanical Engineering
*
* Licensed under the EUPL, Version 1.1 only (the "Licence");
* You may not use this work except in compliance with the Licence.
* You may obtain a copy of the Licence at:
*
* http://ec.europa.eu/idabc/eupl 
*
* Unless required by applicable law or agreed to in writing, software 
* distributed under the Licence is distributed on an "AS IS" basis,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the Licence for the specific language governing permissions and 
* limitations under the Licence.
*/

#ifndef KDL_EXPRESSIONTREE_N_ARY_920309_092098_HPP
#define KDL_EXPRESSIONTREE_N_ARY_920309_092098_HPP

#include <expressiongraph/expressiontree_expressions.hpp>

#include <algorithm>

#include <boost/pointer_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/get_pointer.hpp>
#include <boost/make_shared.hpp>





namespace KDL {
using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;

typedef std::vector<ExpressionBase::Ptr> Arguments;

/**
 * This class is never exposed directly to users. So no typechecking/consistency checking 
 * of the calls.
 */
template< typename _ResultType>
class N_aryExpression: public Expression<_ResultType> {
public:
    Arguments arguments;
    typedef _ResultType ResultType;
    typedef typename AutoDiffTrait<ResultType>::DerivType DerivType;

    N_aryExpression(const std::string& name, int size):Expression<ResultType>(),arguments(size) {}

    virtual void setArgument(int idx, ExpressionBase::Ptr arg) {
        arguments[idx]=arg;
    }
    virtual ExpressionBase::Ptr getArgument(int idx) {
        return arguments[idx];
    }
    virtual int nrOfArguments() {
        return arguments.size();
    }

    virtual void setInputValues(const std::vector<double>& values) {
        for (unsigned int i=0;i<arguments.size();++i) {
            arguments[i]->setInputValues(values); 
        }
    }

    virtual void setInputValue(int variable_number, double val) {
        for (unsigned int i=0;i<arguments.size();++i) {
            arguments[i]->setInputValue(variable_number,val); 
        }
    }

    virtual void setInputValue(int variable_number, const Rotation& val) {
        for (unsigned int i=0;i<arguments.size();++i) {
            arguments[i]->setInputValue(variable_number,val); 
        }
    }

    virtual int number_of_derivatives() {
        int n=0;
        for (unsigned int i=0;i<arguments.size();++i) {
            n = std::max( n, arguments[i]->number_of_derivatives() ); 
        }
        return n;
 
    } 

    virtual typename Expression<Frame>::Ptr subExpression_Frame(const std::string& name) {
        typename Expression<Frame>::Ptr a;
        for (unsigned int i=0;i<arguments.size();++i) {
            a = arguments[i]->subExpression_Frame(name);
            if (a) {
                return a;
            }
        }
        return a;
    }

    virtual typename Expression<Rotation>::Ptr subExpression_Rotation(const std::string& name) {
        typename Expression<Rotation>::Ptr a;
        for (unsigned int i=0;i<arguments.size();++i) {
            a = arguments[i]->subExpression_Rotation(name);
            if (a) {
                return a;
            }
        }
        return a;
    }

    virtual typename Expression<Vector>::Ptr subExpression_Vector(const std::string& name) {
        typename Expression<Vector>::Ptr a;
        for (unsigned int i=0;i<arguments.size();++i) {
            a = arguments[i]->subExpression_Vector(name);
            if (a) {
                return a;
            }
        }
        return a;
    }

    virtual typename Expression<Wrench>::Ptr subExpression_Wrench(const std::string& name) {
        typename Expression<Wrench>::Ptr a;
        for (unsigned int i=0;i<arguments.size();++i) {
            a = arguments[i]->subExpression_Wrench(name);
            if (a) {
                return a;
            }
        }
        return a;
    }

    virtual typename Expression<Twist>::Ptr subExpression_Twist(const std::string& name) {
        typename Expression<Twist>::Ptr a;
        for (unsigned int i=0;i<arguments.size();++i) {
            a = arguments[i]->subExpression_Twist(name);
            if (a) {
                return a;
            }
        }
        return a;

    }

    virtual typename Expression<double>::Ptr subExpression_Double(const std::string& name) {
        typename Expression<double>::Ptr a;
        for (unsigned int i=0;i<arguments.size();++i) {
            a = arguments[i]->subExpression_Double(name);
            if (a) {
                return a;
            }
        }
        return a;
    }

    virtual void addToOptimizer(ExpressionOptimizer& opt) {
        for (unsigned int i=0;i<arguments.size();++i) {
            arguments[i]->addToOptimizer(opt);
        }
    }


    virtual void getDependencies(std::set<int>& varset) {
        for (unsigned int i=0;i<arguments.size();++i) {
            arguments[i]->getDependencies(varset);
        }
    }

    virtual void getScalarDependencies(std::set<int>& varset) {
        for (unsigned int i=0;i<arguments.size();++i) {
            arguments[i]->getScalarDependencies(varset);
        }
    }

    virtual void getRotDependencies(std::set<int>& varset) {
        for (unsigned int i=0;i<arguments.size();++i) {
            arguments[i]->getRotDependencies(varset);
        }
    }

    virtual void debug_printtree() {
        std::cout << Expression<ResultType>::name << "(";
        for (unsigned int i=0;i<arguments.size();++i) {
            if (i!=0) std::cout << ",";
            arguments[i]->debug_printtree();
        }
        std::cout << ")";
    }
    virtual void print(std::ostream& os) const {
        os << "" << Expression<ResultType>::name << "(";
        for (unsigned int i=0;i<arguments.size();++i) {
            if (i!=0) os << ",";
            arguments[i]->print(os);
        }
        os << ")";
    }

    virtual void update_variabletype_from_original() {
        for (unsigned int i=0;i<arguments.size();++i) {
            arguments[i]->update_variabletype_from_original();
        }
    }

    virtual void write_dotfile_update(std::ostream& of, pnumber& thisnode) {
        thisnode=(size_t)this;
        of << "S"<<thisnode<<"[label=\"" << Expression<ResultType>::name << "\",shape=box,style=filled,fillcolor="
           << COLOR_OPERATION << ",color=black]\n";
        std::vector<pnumber> argnode( arguments.size());
        for (unsigned int i=0;i<arguments.size();++i) {
            arguments[i]->write_dotfile_update(of,argnode[i]);
        }
        for (unsigned int i=0;i<arguments.size();++i) {
            of << "S"<<thisnode<<" -> " << "S"<<argnode[i] << "\n"; //rev
        }
    }
    virtual void write_dotfile_init() {
        for (unsigned int i =0;i<arguments.size();++i) {
            arguments[i]->write_dotfile_init();
        }
    } 

    virtual ~N_aryExpression() {
    }
};

} // end of namespace KDL
#endif
