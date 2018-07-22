//
//  DataAlias.hpp
//  pricing_moderls
//
//  Created by Hankun Zhang on 09/07/2018.
//  Copyright © 2018 Julius Cheung. All rights reserved.
//

#ifndef DataAlias_hpp
#define DataAlias_hpp

#include <stdio.h>
#include <vector>
#include <random>
#include <iostream>
#include <cmath>

using namespace std;

namespace alias
{
    inline double max(double a, double b)
    {
        return (a > b ? a : b);
    }
    
    int const laguerreOrder = 5;
    
    using B = bool;
    using R = long double;
    
    using VecR = std::vector<R>;
    using VecVR = std::vector<VecR>;
    using VecRItr = std::vector<R>::iterator;

    using TimeNanos = std::int64_t;
    
    class DateTime
    {
    public:
        DateTime(TimeNanos timeN);
        ~DateTime();
        void const Display();
        TimeNanos getTime() const;
        
        static const TimeNanos nanosPerYear = 365LL * 24LL * 3600LL * 1000000000LL;
        
        TimeNanos operator- (const DateTime& d) const;
    protected:
        TimeNanos timeN_;
    };
    
    inline TimeNanos DateTime::operator- (const DateTime& d) const
    {
        return (this->timeN_ - d.timeN_);
    }
    
    using VecDateTime = std::vector<DateTime>;
    using VecDateTimeItr = std::vector<DateTime>::iterator;
    
    class Matrix
    {
    public:
        Matrix(int height, int width);
        ~Matrix();
        int getHeight() const;
        int getWidth() const;
        VecVR getMatrix() const;
        VecR getCol(int col) const;
        VecR getRow(int row) const;
        R getEntry(int row, int column) const;
        void setEntry(int row, int column, R value);
        void display() const;
        //validation methods for matrix multiplication
        B validateDim(Matrix mat) const;
        B validateDim(VecR vec) const;
        R getDet();
        Matrix getInverse();    //recursive method, numerically unstable
        Matrix invertLU();
        std::vector<Matrix> factorLU();  //LU decomposition
        
     //   Matrix & operator = (const Matrix& mat) const;
        Matrix operator = (const VecR &vec) const;
        Matrix operator * (const Matrix& mat) const;
        VecR operator * (const VecR& vec) const;
        Matrix operator * (const R& scalar) const;
        Matrix operator + (const Matrix& mat) const;
        Matrix operator - (const Matrix& mat) const;
        Matrix operator / (const R& scalar) const;
    private:
        int height_;
        int width_;
        VecVR matrix_;
        Matrix getMinorMatrix(int row, int column);
    };
    
    inline void displayVector(VecR vec);
    
    inline Matrix  vecToMatrix(VecR vec);
    inline VecR colVecToVec(Matrix mat);
    inline R dotProduct( VecR lhs,  VecR rhs);
    inline Matrix transpose(Matrix mat);
    inline VecR scaleVec(VecR vec, R scalar);
    inline VecR sumVec(VecR lhs, VecR rhs);
    inline void copyVec(VecR &lhs, VecR rhs); //copying the rhs vector to lhs
    inline void copyColVec (VecR &vec, Matrix mat, int col);
    inline void setColVec (VecR vec, Matrix &mat, int col);
    inline void copyRowVec (VecR &vec, Matrix mat, int row);
    inline void setRowVec (VecR vec, Matrix &mat, int row);
    
    inline void matSwitchCols(Matrix &mat, const int col1, const int con2);
    inline void matSwitchRows(Matrix &mat, const int row1, const int row2);
    
    inline Matrix getSubMat(Matrix mat, int row, int col, int height, int width);
    inline void insertSubMat(Matrix &mat, Matrix matToInsert, int row, int col, int height, int width);
    inline void validateSubMatRange(int matHeight, int matWidth, int row, int col, int height, int width);
    
    inline void validateSquareMat(Matrix mat);
    inline void validateTriangularMat(Matrix mat, B isUpper);
    inline Matrix invertTriangularMat(Matrix mat, B isUpper);
    
    inline VecR colVecToMat(Matrix mat)
    {
        if (mat.getWidth() != 1)
        {
            throw runtime_error("Not a column vector.");
        }
        VecR result (mat.getHeight());
        for (int i = 0; i < mat.getHeight(); i++)
        {
            result[i] = mat.getEntry(i, 0);
        }
        return result;
    }
    
    inline Matrix vecToMatrix(VecR vec)
    {
        Matrix result(vec.size(), 1);
        for (int i = 0; i < vec.size(); i++)
        {
            result.setEntry(i, 0, vec[i]);
        }
        return result;
    }
    
    inline R dotProduct( VecR lhs,  VecR rhs)
    {
        if (!(lhs.size() == rhs.size()))
        {
            throw std::runtime_error("Invalid dot product operation.");
        }
        R result = 0;
        for (int i = 0; i < lhs.size(); i++)
        {
            result += (lhs[i] * rhs[i]);
        }
        return result;
    }
    
    inline Matrix transpose(Matrix mat)
    {
        int height = mat.getHeight();
        int width = mat.getWidth();
        Matrix result(width, height);
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                result.setEntry(i, j, mat.getEntry(j, i));
            }
        }
        return result;
    }

    inline void displayVector(VecR vec)
    {
        std::cout << "Displaying content of the vector: " << endl;
        
        for (int i = 0; i < vec.size(); i++)
        {
            std::cout << vec[i] << " ";
        }
        std::cout << endl;
    }

    inline VecR scaleVec(VecR vec, R scalar)
    {
        VecR result (vec.size());
        for (int i = 0; i < vec.size(); i++)
        {
            result[i] = vec[i] * scalar;
        }
        return result;
    }
    
    inline VecR sumVec(VecR lhs, VecR rhs)
    {
        if (lhs.size() != rhs.size()) throw runtime_error("Vectors not of the same dimension.");
        VecR result (rhs.size());
        for (int i = 0; i < lhs.size(); i++)
        {
            result[i] = lhs[i] + rhs[i];
        }
        return result;
    }
    
    inline void copyVec(VecR &lhs, VecR rhs)
    {
        if (lhs.size() != rhs.size()) throw runtime_error("Vectors not of the same dimension.");
        for (int i = 0; i < lhs.size(); i++)
        {
            lhs[i] = rhs[i];
        }
    }
    
    inline void copyColVec(VecR &vec, Matrix mat, int col)
    {
        if (vec.size() != mat.getHeight())
        {
            throw runtime_error("Target vector and column vector not of the same dimension");
        }
        for (int i = 0; i < vec.size(); i++)
        {
            vec[i] = mat.getEntry(i, col);
        }
    }
    
    inline void setColVec(VecR vec, Matrix &mat, int col)
    {
        if (vec.size() != mat.getHeight())
        {
            throw runtime_error("Target vector and column vector not of the same dimension");
        }
        for (int i = 0; i < vec.size(); i++)
        {
            mat.setEntry(i, col, vec[i]);
        }
    }
    
    inline void copyRowVec(VecR &vec, Matrix mat, int row)
    {
        if (vec.size() != mat.getWidth())
        {
            throw runtime_error("Target vector and row vector not of the same dimension");
        }
        for (int i = 0; i < vec.size(); i++)
        {
            vec[i] = mat.getEntry(row, i);
        }
    }
    
    inline void setRowVec(VecR vec, Matrix &mat, int row)
    {
        if (vec.size() != mat.getWidth())
        {
            throw runtime_error("Target vector and row vector not of the same dimension");
        }
        for (int i = 0; i < vec.size(); i++)
        {
            mat.setEntry(row, i, vec[i]);
        }
    }
    
    inline void matSwitchCols(Matrix &mat, const int col1, const int col2)
    {
        if ((col1 < 0 || col1 >= mat.getWidth()) || (col2 < 0 || col2 >= mat.getWidth()))
        {
            throw runtime_error("Error: columns out of range.");
        }
        VecR temp (mat.getHeight());
        for (int i = 0; i < temp.size(); i++)
        {
            temp[i] = mat.getEntry(i, col2);
            mat.setEntry(i, col2, mat.getEntry(i, col1));
            mat.setEntry(i, col1, temp[i]);
        }
    }
    
    inline void matSwitchRows(Matrix &mat, const int row1, const int row2)
    {
        if ((row1 < 0 || row1 >= mat.getHeight()) || (row2 < 0 || row2 >= mat.getHeight()))
        {
            throw runtime_error("Error: rows out of range.");
        }
        VecR temp (mat.getWidth());
        for (int i = 0; i < temp.size(); i++)
        {
            temp[i] = mat.getEntry(row2, i);
            mat.setEntry(row2, i, mat.getEntry(row1, i));
            mat.setEntry(row1, i, temp[i]);
        }
    }

    inline Matrix getSubMat(Matrix mat, int row, int col, int height, int width)
    {
        validateSubMatRange(mat.getHeight(), mat.getWidth(), row, col, height, width);
        Matrix subMat(height, width);
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                subMat.setEntry(i, j, mat.getEntry(row + i, col + j));
            }
        }
        return subMat;
    }
    
    inline void insertSubMat(Matrix &mat, Matrix matToInsert, int row, int col, int height, int width)
    {
        validateSubMatRange(mat.getHeight(), mat.getWidth(), row, col, height, width);
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                mat.setEntry(row + i, col + j, matToInsert.getEntry(i, j));
            }
        }
    }

    inline void validateSubMatRange(int matHeight, int matWidth, int row, int col, int height, int width)
    {
        if ((row < 0 || row >= matHeight) || (col < 0 || col >= matWidth) )
        {
            throw runtime_error("Error: invalid leftmost position.");
        }
        int rightMostCol = col + width - 1;
        int downMostRow = row + height - 1;
        if (rightMostCol >= matWidth || downMostRow >= matHeight)
        {
            throw runtime_error("Error: sub-matrix out of bound.");
        }
    }
    
    
    inline void validateSquareMat(Matrix mat)
    {
        if (mat.getWidth() != mat.getHeight())
        {
            throw runtime_error("Validation error: not a square matrix.");
        }
    }
    
    inline void validateTriangularMat(Matrix mat, B isUpper)
    {
        validateSquareMat(mat); //if not a square matrix, throw runtime error
        if (!isUpper) //Lower triangular
        {
            for (int row = 0; row < mat.getHeight() - 1; row++)
            {
                for (int col = row + 1; col < mat.getWidth(); col++)
                {
                    if (mat.getEntry(row, col) != 0)
                    {
                        throw runtime_error("Validation error: not an upper triangular matrix.");
                    }
                }
            }
        } else  //Upper triangular
        {
            for (int row = 1; row < mat.getHeight(); row++)
            {
                for (int col = row - 1; col >= 0; col--)
                {
                    if (mat.getEntry(row, col) != 0)
                    {
                        throw runtime_error("Validation error: not a lower triangular matrix.");
                    }
                }
            }
        }
        for (int row = 0; row < mat.getHeight(); row++)
        {
            if (mat.getEntry(row, row) == 0)    //check no 0 entry on the main diagonal
            {
                throw runtime_error("Validatoin error: singular lower triangular matrix.");
            }
        }
    }
    
    inline Matrix invertTriangularMat(Matrix mat, B isUpper)
    {
        validateTriangularMat(mat, isUpper);
        Matrix inverse(mat.getHeight(), mat.getWidth());    //each entry initialised as 0
        int row;
        int col;
        R entry;
        R num;
        VecR A (mat.getWidth());
        VecR B (mat.getHeight());
        
        //Inverting entries on the main diagonal
        for (int i = 0; i < mat.getHeight(); i++)
        {
            inverse.setEntry(i, i, 1.0 / mat.getEntry(i, i));
        }
        if (isUpper)    //Upper triangular matrix
        {
            for (int colLast = 1; colLast < mat.getWidth(); colLast++)
            {
                col = colLast;
                row = 0;
                while (col < mat.getWidth())
                {
                    //calculate entray (row, col)
                    copyRowVec(A, mat, row);
                    copyColVec(B, inverse, col);
                    num = A[row];
                    entry = (0 - dotProduct(A, B)) / num;
                    inverse.setEntry(row, col, entry);
                    row++;
                    col++;
                }
            }
        } else  //Lower triangular matrix
        {
            for (int rowLast = 1; rowLast < mat.getHeight(); rowLast++)
            {
                row = rowLast;
                col = 0;
                while (row < mat.getHeight())
                {
                    //calculate entray (row, col)
                    copyRowVec(A, mat, row);
                    copyColVec(B, inverse, col);
                    num = A[row];
                    entry = (0 - dotProduct(A, B)) / num;
                    inverse.setEntry(row, col, entry);
                    row++;
                    col++;
                }
            }
        }
        return inverse;
    }

    using Gen = std::default_random_engine;
    using Norm = std::normal_distribution<double>;
} // namespace alias
#endif /* DataAlias_hpp */
