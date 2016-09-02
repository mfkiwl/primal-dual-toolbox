// This file is part of primal-dual-toolbox.
//
// Copyright (C) 2018 Kerstin Hammernik <hammernik at icg dot tugraz dot at>
// Institute of Computer Graphics and Vision, Graz University of Technology
// https://www.tugraz.at/institute/icg/research/team-pock/
//
// primal-dual-toolbox is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or any later version.
//
// primal-dual-toolbox is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <memory>

#include "primaldualtoolbox_api.h"

#include "operator/operatorbase.h"
#include "optimizer_helper.h"

#include <iu/iumath/typetraits.h>
#include <iu/iumath.h>
#include <iu/iucore.h>

/** \brief Parameters for the TgvOptimizer optimizer.
 */
template<typename PixelType>
struct PRIMALDUALTOOLBOX_DLLAPI TgvParameters
{
  /** Constructor. */
  TgvParameters();

  /** Destructor. */
  ~TgvParameters();

  unsigned int check;
  PixelType lambda;
  unsigned int max_iter;
  PixelType alpha0;
  PixelType alpha1;

  /** Print information about the parameters. */
  std::string print() const
  {
    std::stringstream msg;
    msg << "TgvParameters:" << std::endl;
    msg <<  " check=" << check << std::endl;
    msg <<  " lambda=" << lambda << std::endl;
    msg << " alpha0=" << alpha0 << std::endl;
    msg << " alpha1=" << alpha1 << std::endl;
    msg << " max_iter=" << max_iter << std::endl;
    return msg.str();
  }

  /** Overload operator << for printing operator information. */
  friend std::ostream& operator<<(std::ostream& msg,
                                  const TgvParameters<PixelType>& params)
  {
    return msg << params.print();
  }
};

/** \brief Second-order TGV optimizer for denoising.
 *
 *   According to:
 *   K. Bredies, K. Kunisch and T. Pock:
 *   Total generalized variation.
 *   SIAM Journal on Imaging Sciences 3 (3), 492-526 (2010).
 */
template<typename InputType, typename OutputType>
class PRIMALDUALTOOLBOX_DLLAPI TgvOptimizer
{
private:
  typedef typename InputType::pixel_type pixel_type;
  typedef typename OutputType::pixel_type output_pixel_type;

  static const unsigned int ndim = InputType::ndim;
  typedef typename iu::type_trait<pixel_type>::real_type real_type;
  typedef typename iu::type_trait<pixel_type>::complex_type complex_type;

public:
  /** Constructor. */
  TgvOptimizer();

  /** Destructor. */
  virtual ~TgvOptimizer();

  /** Run optimizer. */
  void solve(bool verbose = false);

  /** Set initial input. */
  void setInput0(const InputType &input);

  /** Set noisy data. */
  void setNoisyData(const std::shared_ptr<OutputType> &f);

  /** Get result of the optimizer. */
  InputType* getResult();

  /** Get optimizer parameters of type TgvParameters. */
  TgvParameters<real_type>& getParameters()
  {
    return params_;
  }

  /** No copies are allowed. */
  TgvOptimizer(TgvOptimizer const&) = delete;

  /** No assignments are allowed. */
  void operator=(TgvOptimizer const&) = delete;

private:
  /** Optimizer parameters of type TgvParameters. */
  TgvParameters<real_type> params_;

  /** Noisy input data. */
  std::shared_ptr<OutputType> f_;

  /** Primal variable. */
  std::unique_ptr<InputType> u_;

  /** Overrelaxation of primal variable. */
  std::unique_ptr<InputType> u__;

  /** Primal variable. */
  Variable2<InputType> v_;

  /** Overrelaxation of primal variable. */
  Variable2<InputType> v__;

  /** Dual variable. */
  Variable2<InputType> p_;

  /** Dual variable. */
  Variable2sym<InputType> q_;
};
