//
//  DataAlias.cpp
//  pricing_moderls
//
//  Created by Hankun Zhang on 09/07/2018.
//  Copyright © 2018 Julius Cheung. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "DataAlias.hpp"

using namespace alias;

DateTime::DateTime(TimeNanos timeN)
{
    timeN_ = timeN;
}

DateTime::~DateTime() {};

void const DateTime::Display()
{
    std::cout << "Time to expiry in nanosecodns: " << timeN_ << endl;
}

TimeNanos DateTime::getTime() const
{
    return timeN_;
}

Matrix::Matrix(int height, int width):height_(height), width_(width)
{
    VecVR matrix (height_);
    for (int i = 0; i < height_; i++)
    {
        VecR columnVec (width_);
        matrix[i] = columnVec;
    }
    matrix_ = matrix;
}
Matrix::~Matrix() {}

int Matrix::getHeight() const {return height_;}
int Matrix::getWidth() const {return width_;}
VecVR Matrix::getMatrix() const {return matrix_;}
R Matrix::getEntry(int row, int column) const {return matrix_[row][column];}
void Matrix::setEntry(int row, int column, R value)
{
    matrix_[row][column] = value;
}

void Matrix::display() const
{
    std::cout << "Displaying content of the matrix: " << endl;
    for (int i = 0; i < height_; i++)
    {
        for (int j = 0; j < width_; j++)
        {
            std::cout << left << setw(15) << matrix_[i][j];
        }
        std::cout << endl;
    }
    std::cout << endl << ">";
}
B Matrix::validateDim(alias::Matrix mat) const
{
    if (this->width_ == mat.height_) return true;
    return false;
}
B Matrix::validateDim(VecR vec) const
{
    if (this->width_ == vec.size()) return true;
    return false;
}

Matrix Matrix::operator=(const VecR &vec) const
{
    if(width_ != 1 || height_ != vec.size())
    {
        throw runtime_error("Error: incompitable dimensions.");
    }
    Matrix result(vec.size(), 1);
    for (int i = 0; i < vec.size(); i++)
    {
        result.setEntry(i, 0, vec[i]);
    }
    return result;
}

Matrix Matrix::operator*(const alias::Matrix &mat) const
{
    int height, width;
    height = this->height_;
    width = mat.width_;
    Matrix result(height, width);
    
    if(!(this->validateDim(mat)))
    {
        std::cout << "Matrix multiplication error." << endl;
        return result; //if invalid, return a zero matrix
    }
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            R entry = 0;
            for (int k = 0; k < this->width_; k++)
            {
                entry += (matrix_[i][k] * mat.matrix_[k][j]);
            }
            result.matrix_[i][j] = entry;
        }
    }
    return result;
}

VecR Matrix::operator*(const VecR &vec) const
{
    VecR result (vec.size());
    
    if(!(this->validateDim(vec)))
    {
        std::cout << "Matrix multiplication error." << endl;
        return result; //if invalid, return a zero vector
    }
    
    for (int i = 0; i < vec.size(); i++ )
    {
        //calculate ith entry in the vector
        R entry = 0;
        for (int k = 0; k < vec.size(); k++)
        {
            entry += matrix_[i][k] * vec[k];
        }
        result[i] = entry;
    }
    return result;
}

//scalar multiplication
Matrix Matrix::operator*(const R &scalar) const
{
    Matrix result(height_, width_);
    for (int row = 0; row < height_; row++)
    {
        for(int col = 0; col < width_; col++)
        {
            result.setEntry(row, col, this->getEntry(row, col) * scalar);
        }
    }
    return result;
}


Matrix Matrix::operator+(const alias::Matrix &mat) const
{
    Matrix result(height_, width_);
    
    if (!(height_ == mat.height_ && width_ == mat.width_))
    {
        std::cout << "Matrix addition error." << endl;
        return result; //if invalid, return a zero matrix
    }
    
    for (int i = 0; i < height_; i++)
    {
        for (int j = 0; j < width_; j++)
        {
            result.matrix_[i][j] = matrix_[i][j] + mat.matrix_[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator-(const alias::Matrix &mat) const
{
    Matrix result(height_, width_);
    
    if (!(height_ == mat.height_ && width_ == mat.width_))
    {
        std::cout << "Matrix addition error." << endl;
        return result; //if invalid, return a zero matrix
    }
    
    for (int i = 0; i < height_; i++)
    {
        for (int j = 0; j < width_; j++)
        {
            result.matrix_[i][j] = matrix_[i][j] - mat.matrix_[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator/(const R &scalar) const
{
    if (scalar == 0)
    {
        throw runtime_error("Error: Division by 0.");
    }
    Matrix result(height_, width_);
    for (int row = 0; row < height_; row++)
    {
        for (int col = 0; col < width_; col++)
        {
            result.setEntry(row, col, this->getEntry(row, col) / scalar);
        }
    }
    return result;
}

R Matrix::getDet()
{
    if (height_ != width_)
    {
        throw runtime_error("A determinant is only defined for a square matrix.");
    }
    int dim = height_;
    R det = 0;  
    if (dim == 1)
    {
        //base case
        det = matrix_[0][0];
    } else
    {
        for (int i = 0; i < dim; i++)
        {
            //recursion
            det += (std::pow(-1,i) * matrix_[0][i] * getMinorMatrix(0, i).getDet());
        }
    }

    return det;
}

Matrix Matrix::getMinorMatrix(int row, int column)
{
    if (row >= height_  || column >= width_)
    {
        throw runtime_error("Entry is not within the matrix.");
    }
    Matrix result(height_ - 1, width_ - 1);
    //Initialising the resultant matrix
    for (int i = 0; i < height_; i++)
    {
        for (int j = 0; j < width_; j++)
        {
            if (i < row)
            {
                if (j < column)
                {
                    result.setEntry(i, j, matrix_[i][j]);
                }
                else
                {
                    if (j > column) result.setEntry(i, j - 1, matrix_[i][j]);
                }
            }
            else
            {
                if (i > row)
                {
                    if (j < column)
                    {
                        result.setEntry(i - 1, j, matrix_[i][j]);
                    }
                    else
                    {
                        if (j > column) result.setEntry(i - 1, j - 1, matrix_[i][j]);
                    }
                }
            }
        }
    }
    return result;
}

Matrix Matrix::invertLU()
{
    std::vector<Matrix> results = this->factorLU();
    Matrix permut = results[0];
    Matrix upper = results[1];
    Matrix lower = results[2];
    Matrix lowInv = invertTriangularMat(lower, false);
    Matrix upInv = invertTriangularMat(upper, true);
    return (invertTriangularMat(upper, true) * invertTriangularMat(lower, false) * permut);
}

Matrix Matrix::getInverse()
{
    Matrix inverse(height_, width_);
    R det = 0;
    R entryDet = 0;
    //calcualte the cofactor matrix
    for (int row = 0; row < height_; row++)
    {
        for (int col = 0; col < width_; col++)
        {
            entryDet = getMinorMatrix(row, col).getDet();
            inverse.setEntry(row, col, std::pow(-1, row + col) * entryDet);
            if (row == 0)
            {
                det += std::pow(-1, col) * matrix_[0][col] * entryDet;
            }
        }
        if (row == 0 && det == 0)
        {
            throw runtime_error("The matrix is singular");
        }
    }
    inverse = inverse / det;
    inverse = transpose(inverse);
    return inverse;
}

std::vector<Matrix> Matrix::factorLU()
{
    //recusive member function to find the LU decomposition
    //recursion formulas given at page 59 of http://www4.ncsu.edu/~ipsen/ps/OT113_Ipsen.pdf
    validateSquareMat(*this);
    std::vector<Matrix> results;
    Matrix permut(height_, width_); //potential efficiency improvement: store permutations in array
    Matrix upper(height_, width_);
    Matrix lower(height_, width_);
    Matrix A = (*this) ;
    
    if (height_ == 1)
    {
        //base case
        permut.setEntry(0, 0, 1);
        lower.setEntry(0, 0, 1);
        upper.setEntry(0, 0, A.getEntry(0, 0));
    } else
    {
        R alpha = 0;
        int row = 0;    //index of column containin alpha to be switched to the front
        Matrix a(1, width_ - 1);
        Matrix l(height_ - 1, 1);
        Matrix S(height_ - 1, width_ - 1);
        Matrix factorP(height_, width_);
        Matrix permuteNext(height_ - 1, width_ - 1);
        Matrix upperNext(height_ - 1, width_ - 1);
        Matrix lowerNext(height_ - 1, width_ - 1);
        std::vector<Matrix> resultsNext;
        
        //initialising permutation matrix for the step
        for (int row = 0; row < height_; row++)
        {
            permut.setEntry(row, row, 1.0);
        }
        
        for (int i = 0; i < height_; i++)
        {
            if (abs(A.getEntry(i, 0)) > alpha)
            {
                alpha = A.getEntry(i, 0);
                row = i;
            }
        }
        matSwitchRows(permut, 0, row);
        A = permut * A;
        S = getSubMat(A, 1, 1, height_ - 1, width_ - 1);
        a = getSubMat(A, 0, 1, 1, width_ - 1);
        l = getSubMat(A, 1, 0, height_ - 1, 1);
        l = l / alpha;
        S = S - (l * a);
        resultsNext = S.factorLU(); //recursion step
    
        permuteNext = resultsNext[0];
        upperNext = resultsNext[1];
        lowerNext = resultsNext[2];
        
        //L and U initialised as zero matrice
        //recursively calculate L
        lower.setEntry(0, 0, 1);
        insertSubMat(lower, (permuteNext * l), 1, 0, height_ - 1, 1);
        insertSubMat(lower, lowerNext, 1, 1, height_ - 1, width_ - 1);
        //recursively calculate U
        upper.setEntry(0, 0, alpha);
        insertSubMat(upper, a, 0, 1, 1, width_ - 1);
        insertSubMat(upper, upperNext, 1, 1, height_ - 1, width_ - 1);
        //recursively caluclate P
        factorP.setEntry(0, 0, 1);
        insertSubMat(factorP, permuteNext, 1, 1, height_ - 1, width_ - 1);
        permut = (factorP * permut);
    }
    
    results.emplace_back(permut);
    results.emplace_back(upper);
    results.emplace_back(lower);
    return results;
}
