// Copyright 2018 Uber Technologies, Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// =============================================================================

#ifndef HOROVOD_GAUSSIAN_PROCESS_H
#define HOROVOD_GAUSSIAN_PROCESS_H

#include <Eigen/Cholesky>
#include <vector>

namespace horovod {
namespace common {

class GaussianProcessRegressor {
public:
  GaussianProcessRegressor(double alpha);

  ~GaussianProcessRegressor() {}

  void Fit(Eigen::MatrixXd* x_train, Eigen::MatrixXd* y_train);

  // Isotropic squared exponential kernel.
  // Computes a covariance matrix from points in X1 and X2.
  //
  // Args:
  //  x1: Matrix of m points (m x d).
  //  x2: Matrix of n points (n x d).
  //
  // Returns: Covariance matrix (m x n).
  Eigen::MatrixXd Kernel(const Eigen::MatrixXd& x1, const Eigen::MatrixXd& x2, double l=1.0, double sigma_f=1.0) const;

  void Predict(const Eigen::MatrixXd& x, Eigen::VectorXd& mu, Eigen::VectorXd* sigma=nullptr) const;

  // Computes the suffifient statistics of the GP posterior predictive distribution
  // from m training data X_train and Y_train and n new inputs X_s.
  //
  // Args:
  //  x_s: New input locations (n x d).
  //  x_train: Training locations (m x d).
  //  y_train: Training targets (m x 1).
  //  l: Kernel length parameter.
  //  sigma_f: Kernel vertical variation parameter.
  //  sigma_y: Noise parameter.
  //
  // Returns: Posterior mean vector (n x d) and covariance matrix (n x n).
  void PosteriorPrediction(const Eigen::MatrixXd& x_s, const Eigen::MatrixXd& x_train, const Eigen::MatrixXd& y_train,
                           Eigen::VectorXd& mu_s, Eigen::MatrixXd& cov_s,
                           double l=1.0, double sigma_f=1.0, double sigma_y=1e-8) const;

private:
  double alpha_;
  double length_;
  double sigma_f_;

  Eigen::MatrixXd* x_train_;
  Eigen::MatrixXd* y_train_;
};

} // namespace common
} // namespace horovod

#endif //HOROVOD_GAUSSIAN_PROCESS_H