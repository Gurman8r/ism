@echo off

rem create hidden engine dirs
if not exist .ism ( mkdir .ism )
attrib +H .ism
if not exist .ism\temporary ( mkdir .ism\temporary )
if not exist .ism\workspace ( mkdir .ism\workspace )

rem pause