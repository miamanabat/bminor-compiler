#!/bin/sh

BMINOR="./bminor"
ARG=$1
tests="tests"
sample_tests="tests/sample_tests"

if [ "$ARG" = "scanner" ] || [ "$ARG" = "all" ] 
then
    for testfile in $tests/scanner/good*.bminor
    do
	    if $BMINOR -scan $testfile
	    then
		    echo "$testfile success (as expected)"
	    else
		    echo "$testfile failure (INCORRECT)"
	    fi
    done

    for testfile in $tests/scanner/bad*.bminor
    do
	    if $BMINOR -scan $testfile
	    then
		    echo "$testfile success (INCORRECT)"
    	else
	    	echo "$testfile failure (as expected)"
	    fi
    done
    for testfile in $tests/scanner/sample_tests/good*.bminor
    do
	    if $BMINOR -scan $testfile
	    then
		    echo "$testfile success (as expected)"
	    else
		    echo "$testfile failure (INCORRECT)"
	    fi
    done

    for testfile in $tests/scanner/sample_tests/bad*.bminor
    do
	    if $BMINOR -scan $testfile
	    then
		    echo "$testfile success (INCORRECT)"
    	else
	    	echo "$testfile failure (as expected)"
	    fi
    done
fi
if [ "$ARG" = "parser" ] || [ "$ARG" = "all" ] 
then
    for testfile in $tests/parser/good*.bminor
    do
	    if $BMINOR -parse $testfile
	    then
		    echo "$testfile success (as expected)"
	    else
		    echo "$testfile failure (INCORRECT)"
	    fi
    done

    for testfile in $tests/parser/bad*.bminor
    do
	    if $BMINOR -parse $testfile
	    then
		    echo "$testfile success (INCORRECT)"
    	else
	    	echo "$testfile failure (as expected)"
	    fi
    done

    for testfile in $tests/parser/sample_tests/good*.bminor
    do
	    if $BMINOR -parse $testfile
	    then
		    echo "$testfile success (as expected)"
	    else
		    echo "$testfile failure (INCORRECT)"
	    fi
    done

    for testfile in $tests/parser/sample_tests/bad*.bminor
    do
	    if $BMINOR -parse $testfile
	    then
		    echo "$testfile success (INCORRECT)"
    	else
	    	echo "$testfile failure (as expected)"
	    fi
    done
fi
if [ "$ARG" = "printer" ] || [ "$ARG" = "all" ] 
then
    for testfile in $tests/printer/good*.bminor
    do
        if $BMINOR -print $testfile > output1
        then
            $BMINOR -print output1 > output2
	        if cmp -s output1 output2
	        then
		        echo "$testfile success (as expected)"
	        else
		        echo "$testfile failure (INCORRECT)"
	        fi
        else
            echo "$testfile success (INCORRECT)"
        fi
    done

    for testfile in $tests/printer/bad*.bminor
    do
        if $BMINOR -print $testfile > output1
        then
            echo "$testfile success (INCORRECT)"
        else
            echo "$testfile failure (as expected)"
        fi
    done

    for testfile in $tests/printer/sample_tests/good*.bminor
    do
        if $BMINOR -print $testfile > output1
        then
            $BMINOR -print output1 > output2
	        if cmp -s output1 output2
	        then
		        echo "$testfile success (as expected)"
	        else
		        echo "$testfile failure (INCORRECT)"
	        fi
        else
            echo "$testfile success (INCORRECT)"
        fi
    done

    for testfile in $tests/printer/sample_tests/bad*.bminor
    do
        if $BMINOR -print $testfile > output1
        then
            echo "$testfile success (INCORRECT)"
        else
            echo "$testfile failure (as expected)"
        fi
    done
fi
if [ "$ARG" = "typecheck" ] || [ "$ARG" = "all" ] 
then
    for testfile in $tests/typecheck/good*.bminor
    do
    	if $BMINOR -typecheck $testfile > /dev/null
	    then
		    echo "$testfile success (as expected)"
    	else
	    	echo "$testfile failure (INCORRECT)"
	    fi
    done

    for testfile in $tests/typecheck/bad*.bminor
    do
	    if $BMINOR -typecheck $testfile > /dev/null
	    then
		    echo "$testfile success (INCORRECT)"
	    else
		    echo "$testfile failure (as expected)"
	    fi
    done
    for testfile in $tests/typecheck/sample_tests/good*.bminor
    do
    	if $BMINOR -typecheck $testfile > /dev/null
	    then
		    echo "$testfile success (as expected)"
    	else
	    	echo "$testfile failure (INCORRECT)"
	    fi
    done

    for testfile in $tests/typecheck/sample_tests/bad*.bminor
    do
	    if $BMINOR -typecheck $testfile > /dev/null
	    then
		    echo "$testfile success (INCORRECT)"
	    else
		    echo "$testfile failure (as expected)"
	    fi
    done
fi
if [ "$ARG" = "codegen" ] || [ "$ARG" = "all" ] 
then
    for testfile in $tests/codegen/good*.bminor
    do 
        if ./bin/bmake.sh $testfile > /dev/null 2>&1 ; then 
            echo "$testfile: success!"
        else
            echo "$testfile: failure!"
        fi
    done
    for testfile in $tests/codegen/sample_tests/good*.bminor
    do 
        if ./bin/bmake.sh $testfile > /dev/null 2>&1 ; then 
            echo "$testfile: success!"
        else
            echo "$testfile: failure!"
        fi
    done
fi
