from sklearn.svm import SVC, LinearSVC
from sklearn.model_selection import GridSearchCV
from time import time

def ParamSearch( trainX, trainy, precomputed = True, C = None, gamma = None  ):
    
    if precomputed:
      return {'kernel': 'rbf',
              'gamma': 0.00397,
              'C': 100,
              'coef0': 0.0,
              'degree' : 3 }
    # fit the model
    t0 = time()
#     kernel = [ 'linear', 'rbf', 'sigmoid', 'poly' ]
#     kernel = [ 'rbf', 'sigmoid', 'poly' ]
#     kernel = [ 'rbf', 'poly' ]
    kernel = 'rbf'
    coef0 = 0.0 # as this is only valid for kernel type 'poly' and 'sigmoid'
#     degree = [ 2, 3, 4, 5, 6 ] # only used in kernel type 'poly'
    degree = [ 3 ]
    C = [ 1e-2, 1e-1, 1e0, 1e1, 2e1, 5e1, 8e1, 1e2, 1e3 ]
    gamma = [ 1e-3, 2e-3, 5e-3, 1e-2, 5e-2, 1e-1 ]
    decision_function_shape = "ovo"
    class_weight = "balanced",
    probability = "True"
    keep_zipmap = "True"

    param_grid = {'C': C,
                  'gamma': gamma,
                  'degree': degree
                 }    
    
    clf_est = SVC( kernel = kernel, coef0 = coef0, degree = degree,
                   decision_function_shape = decision_function_shape, random_state=None)
    gs_clf = GridSearchCV(clf_est, param_grid, verbose = 0)
    gs_clf.fit(trainX, trainy)

    print("done in %0.3fs" % (time() - t0))
    print("Best estimator found by grid search on the training set phase 1: ")
    print(gs_clf.best_estimator_.gamma)
    print(gs_clf.best_estimator_.C)
    
    
    t0 = time()
#     return gs_clf
    best_gamma = gs_clf.best_estimator_.gamma
    best_c = gs_clf.best_estimator_.C
    
    fact_neg = 10**(-0.1)
    fact_pos = 10**(0.1)
    fact_m5 = fact_neg**5
    fact_m4 = fact_neg**4
    fact_m3 = fact_neg**3
    fact_m2 = fact_neg**2
    fact_m1 = fact_neg**1
    fact_m0 = 1
    fact_p1 = fact_pos
    fact_p2 = fact_pos**2
    fact_p3 = fact_pos**3
    fact_p4 = fact_pos**4
    fact_p5 = fact_pos**5
    gamma = [best_gamma * fact_m5, best_gamma * fact_m4, best_gamma * fact_m3, best_gamma * fact_m2,
                   best_gamma * fact_m1, best_gamma * fact_m0, best_gamma * fact_p1, best_gamma * fact_p2,
                   best_gamma * fact_p3, best_gamma * fact_p4, best_gamma * fact_p5]
    C = [best_c * fact_m5, best_c * fact_m4, best_c * fact_m3, best_c * fact_m2, best_c * fact_m1,
               best_c * fact_m0, min(best_c * fact_p1, 1), min(best_c * fact_p2, 1), min(best_c * fact_p3, 1), min(best_c * fact_p4, 1),
               min(best_c * fact_p5, 1)]

    param_grid2 = {'C': C,
                  'gamma': gamma,
                  'degree': degree
                 }  
    clf_est = SVC( kernel = 'rbf', coef0 = coef0, degree = degree,
                   decision_function_shape = decision_function_shape, random_state=None)

    gs_clf = GridSearchCV(clf_est, param_grid2, verbose = 0)
    gs_clf.fit( trainX, trainy )

    print("done in %0.3fs" % (time() - t0))
    print("Best estimator found by grid search on the training set phase 2: ")
    print(gs_clf.best_estimator_.gamma)
    print(gs_clf.best_estimator_.C)
    return { 'kernel': gs_clf.best_estimator_.kernel,
             'C': gs_clf.best_estimator_.C,
             'gamma': gs_clf.best_estimator_.gamma,
             'coef0': 0.0,
             'degree': 3,  }
