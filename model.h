#pragma once
#include <cstdarg>
namespace Eloquent {
    namespace ML {
        namespace Port {
            class RandomForest {
                public:
                    /**
                    * Predict class for features vector
                    */
                    int predict(float *x) {
                        uint8_t votes[4] = { 0 };
                        // tree #1
                        if (x[1] <= 3150.0) {
                            votes[0] += 1;
                        }

                        else {
                            if (x[0] <= 2585.0) {
                                if (x[4] <= 950.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }

                            else {
                                votes[3] += 1;
                            }
                        }

                        // tree #2
                        if (x[5] <= 3000.0) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[4] <= -2150.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[4] <= 950.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #3
                        if (x[4] <= 9740.0) {
                            if (x[5] <= 8000.0) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[5] <= 10665.0) {
                                    votes[2] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            votes[3] += 1;
                        }

                        // tree #4
                        if (x[2] <= 1896.0) {
                            if (x[0] <= 2585.0) {
                                votes[2] += 1;
                            }

                            else {
                                if (x[4] <= 6650.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[3] += 1;
                                }
                            }
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #5
                        if (x[5] <= 3010.0) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[3] <= 11250.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 1882.5) {
                                    votes[2] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #6
                        if (x[3] <= 4965.0) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[5] <= 10700.0) {
                                if (x[2] <= 1884.0) {
                                    votes[2] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #7
                        if (x[4] <= -2150.0) {
                            votes[0] += 1;
                        }

                        else {
                            if (x[4] <= 9750.0) {
                                if (x[1] <= 3199.0) {
                                    if (x[5] <= 8220.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[2] += 1;
                                    }
                                }

                                else {
                                    if (x[3] <= 14710.0) {
                                        votes[2] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }

                            else {
                                votes[3] += 1;
                            }
                        }

                        // tree #8
                        if (x[5] <= 3060.0) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[2] <= 1890.0) {
                                if (x[1] <= 3170.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #9
                        if (x[2] <= 1895.0) {
                            if (x[3] <= 11250.0) {
                                if (x[4] <= 6660.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[3] += 1;
                                }
                            }

                            else {
                                votes[2] += 1;
                            }
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #10
                        if (x[3] <= 5000.0) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[4] <= 985.0) {
                                if (x[5] <= 9690.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                votes[2] += 1;
                            }
                        }

                        // tree #11
                        if (x[4] <= 9750.0) {
                            if (x[3] <= 14500.0) {
                                if (x[1] <= 3168.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            votes[3] += 1;
                        }

                        // tree #12
                        if (x[5] <= 3000.0) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[4] <= 950.0) {
                                if (x[1] <= 3162.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                votes[2] += 1;
                            }
                        }

                        // tree #13
                        if (x[0] <= 2615.0) {
                            if (x[2] <= 1890.0) {
                                if (x[3] <= 6700.0) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            votes[0] += 1;
                        }

                        // tree #14
                        if (x[1] <= 3150.0) {
                            votes[0] += 1;
                        }

                        else {
                            if (x[4] <= 9750.0) {
                                if (x[3] <= 14500.0) {
                                    votes[2] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                votes[3] += 1;
                            }
                        }

                        // tree #15
                        if (x[1] <= 3149.0) {
                            votes[0] += 1;
                        }

                        else {
                            if (x[3] <= 6760.0) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[3] <= 14500.0) {
                                    votes[2] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #16
                        if (x[4] <= 940.0) {
                            if (x[3] <= 12750.0) {
                                votes[0] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[5] <= 4000.0) {
                                votes[3] += 1;
                            }

                            else {
                                votes[2] += 1;
                            }
                        }

                        // tree #17
                        if (x[3] <= 14490.0) {
                            if (x[3] <= 11260.0) {
                                if (x[4] <= 6660.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[3] += 1;
                                }
                            }

                            else {
                                votes[2] += 1;
                            }
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #18
                        if (x[3] <= 14510.0) {
                            if (x[0] <= 2587.0) {
                                votes[2] += 1;
                            }

                            else {
                                if (x[3] <= 5000.0) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #19
                        if (x[4] <= 9750.0) {
                            if (x[4] <= 940.0) {
                                if (x[1] <= 3159.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                votes[2] += 1;
                            }
                        }

                        else {
                            votes[3] += 1;
                        }

                        // tree #20
                        if (x[2] <= 1894.0) {
                            if (x[0] <= 2586.0) {
                                votes[2] += 1;
                            }

                            else {
                                if (x[4] <= 6660.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[3] += 1;
                                }
                            }
                        }

                        else {
                            votes[1] += 1;
                        }

                        // return argmax of votes
                        uint8_t classIdx = 0;
                        float maxVotes = votes[0];

                        for (uint8_t i = 1; i < 4; i++) {
                            if (votes[i] > maxVotes) {
                                classIdx = i;
                                maxVotes = votes[i];
                            }
                        }

                        return classIdx;
                    }

                protected:
                };
            }
        }
    }