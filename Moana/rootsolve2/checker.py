from dmoj.result import CheckerResult


def check(process_output, judge_output, **kwargs):
    import cmath

    try:
        if isinstance(process_output, bytes):
            process_output = process_output.decode('ascii')
        if isinstance(judge_output, bytes):
            judge_output = judge_output.decode('ascii')
        plines = process_output.strip().replace('\r\n', '\n').split('\n')
        jlines = judge_output.strip().replace('\r\n', '\n').split('\n')
        if len(plines) != len(jlines):
            return CheckerResult(False, 0, 'Incorrect number of lines')

        # numbers of process output
        px = []
        for line in plines:
            if line.count(' ') > 1:
                return CheckerResult(False, 0, 'Check your whitespace')
            try:
                re, im = map(float, line.split())
                x = cmath.exp(1j*(re+1j*im))
                if abs(x) > 1e6:  # 10 should be enough
                    raise OverflowError()
                px.append(x)
            except OverflowError:
                px.append(1e6+1e6j)
            except ValueError:
                return CheckerResult(False, 0, 'Please output two real numbers in each line')

        # lines of judge output
        jx = []
        for line in jlines:
            re, im = map(float, line.split())
            jx.append(cmath.exp(1j*(re+1j*im)))

        # for demonstration only
        #for i in range(len(jx)):
        #    for j in range(len(jx)):
        #        if i == j:
        #            continue
        #        assert abs(jx[i]-jx[j]) > 1e-4

        # count the number of matches
        count = 0
        for z in jx:
            for x in px:
                if abs(x-z) < 1e-8:  # absolute error
                    count += 1
                    break

        # calculate points
        points = round((count/len(jlines))**5, 2)
        points *= kwargs['point_value']
        feedback = f"{count}/{len(jlines)}"
        return CheckerResult(True, points, feedback)

    except BaseException as e:
        return CheckerResult(False, 0, '')
