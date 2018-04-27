<?xml version="1.0" encoding="utf-8"?>
<!--
	Copyright (C) 2014, 2018 Vaptistis Anogeianakis <nomad@cornercase.gr>

	This file is part of LostArt.

	LostArt is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	LostArt is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with LostArt.  If not, see <http://www.gnu.org/licenses/>.
-->

<!--
	Two controls are connected with constraints.

	The constraints contain characters that can't be part of
	valid linear constrains which will result in an compile-time
	error message.
-->
<gui-model>
	<controls>
		<control>
			<sides>
				<left>10</left>
				<bottom>10</bottom>
				<right>272.0833</right>
				<top>247.2222</top>
			</sides>
			<borderSize>2</borderSize>
			<name>Screen</name>
			<nameHeight>10</nameHeight>
		</control>
		<control>
			<sides>
				<left>29.61875</left>
				<bottom>200.025</bottom>
				<right>86.38805</right>
				<top>153.1056</top>
			</sides>
			<borderSize>2</borderSize>
			<name>Alice</name>
			<nameHeight>10</nameHeight>
		</control>
		<control>
			<sides>
				<left>155.851</left>
				<bottom>205.6694</bottom>
				<right>225.3141</right>
				<top>117.1222</top>
			</sides>
			<borderSize>2</borderSize>
			<name>Bob</name>
			<nameHeight>10</nameHeight>
		</control>
	</controls>
	<constraints>
		<constraint>
			<text>!!</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>2</control>
				<side>left</side>
			</first-end-point>
			<second-end-point>
				<control>1</control>
				<side>right</side>
			</second-end-point>
			<first-local-side>190.813599</first-local-side>
			<second-local-side>197.813599</second-local-side>
		</constraint>
		<constraint>
			<text>@</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>2</control>
				<side>top</side>
			</first-end-point>
			<second-end-point>
				<control>1</control>
				<side>top</side>
			</second-end-point>
			<first-local-side>120.792969</first-local-side>
			<second-local-side>127.792969</second-local-side>
		</constraint>
	</constraints>
</gui-model>
