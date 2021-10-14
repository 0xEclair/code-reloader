#pragma once

<template T>
class Foo {
public:
    auto get() const {
        return t_;
    }

private:
    T t_;
};